<?php

define("ERP_ACCT_TYPE_ASSET",1);
define("ERP_ACCT_TYPE_LIABILITY",2);
define("ERP_ACCT_TYPE_EQUITY",3);
define("ERP_ACCT_TYPE_REVENUE",4);
define("ERP_ACCT_TYPE_EXPENSE",5);

class AccountDb {
	
	public static function add_constraints(){
		
		$cn = Database::getConnection();
		$type = $cn->databaseType();
		$path = DRUPAL_ROOT . "/" . drupal_get_path("module","erp_account");
		if(file_exists("{$path}/db/{$type}_post_install.sql")){
			$sql = file_get_contents("{$path}/db/{$type}_post_install.sql");
			if($sql != ""){
				$stmt = $cn->prepare($sql);
				if($stmt){
					$stmt->execute();
				} else {
					throw new Exception("Unable to add constraints.");
				}
			}
		} else {
			throw new Exception("Unable to determine database type: '{$type}'");
		}
		
	}
	
	public static function drop_constraints(){

		$cn = Database::getConnection();
		$type = $cn->databaseType();
		$path = DRUPAL_ROOT . "/" . drupal_get_path("module","erp_account");
		if(file_exists("{$path}/db/{$type}_pre_uninstall.sql")){
			$sql = file_get_contents("{$path}/db/{$type}_pre_uninstall.sql");
			if($sql != ""){
				$stmt = $cn->prepare($sql);
				if($stmt){
					$stmt->execute();
				} else {
					throw new Exception("Unable to drop constraints.");
				}
			}
		} else {
			throw new Exception("Unable to determine database type: '{$type}'");
		}
		
	}
	
	public static function account_types_get_all(){
		
		$cn = Database::getConnection();
		$sql = <<<SQL
select id, name
from erp_account_types
SQL;
		$stmt = $cn->prepare($sql);
		if($stmt){
			if($stmt->execute()){
				return $stmt->fetchAll(PDO::FETCH_ASSOC);
			} else {
				throw new Exception("Unable to execute sql command to retrieve account types");
			}
		} else {
			throw new Exception("Unable to prepare sql command to retrieve account types");
		}
	}
	
	public static function account_add($type_id, 
		$client_id, 
		$name, 
		$description,
		$code = "",
		$parent_id = ""){
		
		$parent = $parent_id == "" ? array() : self::accounts_get_account_hierarchy($parent_id);
		
		$transaction = db_transaction();
		
		try {
			$account_id = db_insert("erp_accounts")
				->fields(array(
					"name"=>$name,
					"client_id"=>$client_id,
					"desc"=>$description,
					"code"=>$code
				))
				->execute();
			
			/**
			 * adjacency list model
			 * http://mikehillyer.com/articles/managing-hierarchical-data-in-mysql/
			 */
// 			db_insert("erp_account_hierarchy")
// 				->fields(array(
// 					"client_id"=>$client_id,
// 					"parent_acct_id"=>$parent,
// 					"account_id"=>$account_id
// 				))
// 				->execute();
		
			/**
			 * nested set model
			 * http://mikehillyer.com/articles/managing-hierarchical-data-in-mysql/
			 */
			
			/**
			 * check if there is a client root account,
			 * create if there is none
			 */
			$account_root_id = 0;
			$accounts_root = db_select("erp_accounts","a")
				->fields("a","id")
				->condition("client_id", $client_id, "=")
				->condition("name", "ROOT", "=")
				->execute();
			switch(count($accounts_root)){
				case 0:{
					$account_root_id = db_insert("erp_accounts")
						->fields(array(
							"name"=>"ROOT",
							"desc"=>"ROOT - {$client}",
							"client_id"=>$client_id,
							"code"=>""
						))
						->execute();
					
					db_insert("erp_account_hierarchy")
						->fields(array(
							"client_id"=>$client_id,
							"account_id"=>$account_root_id,
							"left_value"=>1,
							"right_value"=>2
						))
						->execute();
					
					if($parent_id == ""){
						$parent_id = $account_root_id;
						$parent = array(
							"account_id"=>$account_root_id,
							"client_id"=>$client_id,
							"left_value"=>1,
							"right_value"=>2
						);
					}
					
					break;
				}
				case 1:{
					$account_root_id = $accounts_root[0]["id"];
					break;
				}
				default:{
					throw new Exception("Cannot have more than 1 ROOT account");
					break;
				}
			}

			
			// retrieve account at the right
			$accounts_right = db_select("erp_account_hiearchy","a")
				->fields("a")
				->condition("client_id", $client_id, "=")
				->condition("left_value", $parent["right_value"])
				->execute();
			$account_right = $accounts_right[0];
			
			db_insert("erp_account_hierarchy")
				->fields(array(
					"account_id"=>$account_id,
					"left_value"=>$parent["left_value"]+1,
					"right_value"=>$account_right["right_value"]+1
				))
				->execute();
			
			db_update("erp_account_hierarchy")
				->fields(array(
					"left_value"=>"left_value + 1",
					"right_value"=>"right_value + 1"
				))
				->condition("client_id", $client_id, "=")
				->condition("right_value", $account_right["right_value"], ">")
				->execute();
			
			db_insert("erp_account_balances")
				->fields(array(
					"client_id"=>$client_id,
					"account_id"=>$account_id,
					"updated"=>REQUEST_TIME
				))
				->execute();
			
		} catch(Exception $e){
			$transaction->rollback();
			throw $e;
		}
	}
	
	public static function account_update(
		$account_id,
		$client_id,
		$name,
		$desc,
		$code = ""){
		
		db_update("erp_accounts")
		->fields(array(
			"client_id"=>$client_id,
			"name"=>$name,
			"desc"=>$desc,
			"code"=>$code
		))
		->condition("id", $id, "=")
		->execute();
		
	}
	
	public static function snapshot_add($client_id){
		
		$transaction = db_transaction();
		try {

			$snapshot_id = db_insert("erp_account_balances_snapshots_metadata")
				->fields(array(
					"client_id"=>$client_id,
					"created"=>REQUEST_TIME
				))
				->execute();
			
			$qry = db_select("erp_account_balances","a")
				->fields("a", "client_id", "account_id", "debit", "created")
				->addExpression($snapshot_id, "snapshot_id")
				->condition("client_id", $client_id, "=");
			
			db_insert("erp_account_balances_snapshots")
				->from($qry)
				->execute();
			
		}catch(Exception $e){
			$transaction->rollback();
			throw $e;
		}
		
	}
	
	public static function accounts_get_account_hierarchy($account_id){
		
		return db_select("erp_account_hierarchy","a")
			->fields("a")
			->condition("account_id", $account_id, "=")
			->execute()
			->fetch(PDO::FETCH_ASSOC);
		
	}
	
	public static function accounts_get_tree($parent){
		
		
		
	}
	
	/**
	 * add transactions
	 * @param array $transactions
	 * 	each item in the array is an associative array with the following keys
	 * 	account_id
	 * 	debit
	 * 	credit
	 */
	public static function transaction_add($client_id, $desc, $transactions = array()){
		
		if(empty($transactions)){
			throw new Exception("Cannot add empty transactions");
		} else {
			// TODO transaction_add
			
			if(self::validate_transactions($transactions)){
			
				$transaction = db_transaction();
				try {
					
					// obtain transaction id
					$transaction_id = db_insert("erp_account_transaction_metadata")
						->fields(array(
							"client_id"=>$client_id,
							"description"=>check_plain($desc),
							"created"=>REQUEST_TIME
						))
						->execute();
					
					// record transactions
					foreach($transactions as $t){
						db_insert("erp_account_transactions")
						->fields(array(
							"transaction_id"=>$transaction_id,
							"client_id"=>$id,
							"account_id"=>$transaction["account_id"],
							"debit"=>$transaction["debit"],
							"credit"=>$transaction["credit"]
						))
						->execute();
					}
					
				}catch(Exception $e){
					$transaction->rollback();
					throw $e;
				}
			}
			
		}
		
	}
	
	private static function validate_transactions($transactions = array()){

		$debit = 0;
		$credit = 0;
		foreach($transactions as $transaction){
			
			if(!isset($transaction["account_id"])){
				return false;
			}
			
			if($transaction["debit"] > 0 && $transaction["credit"] > 0){
				return false;
			}
			
			$debit += $transaction["debit"];
			$credit += $transaction["credit"];
			
		}
		
		if($debit != $credit){
			return false;
		}
		
		return true;
		
	}
	
}