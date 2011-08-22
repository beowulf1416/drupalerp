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

		die("drop_constraints");
		
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
		$code = ""){
		
		db_insert("erp_accounts")
		->fields(array(
			"name"=>$name,
			"client_id"=>$client_id,
			"desc"=>$description,
			"code"=>$code
		))
		->execute();
		
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
	
}