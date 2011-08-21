<?php

define("ERP_ACCT_TYPE_ASSET",1);
define("ERP_ACCT_TYPE_LIABILITY",2);
define("ERP_ACCT_TYPE_EQUITY",3);
define("ERP_ACCT_TYPE_REVENUE",4);
define("ERP_ACCT_TYPE_EXPENSE",5);

class AccountDb {
	
	public static function modify_tables(){
		
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
					throw new Exception("Unable to modify tables.");
				}
			}
		} else {
			throw new Exception("Unable to determine database type: '{$type}'");
		}
		
	}
	
	public static function account_add($type_id, 
		$client_id, 
		$name, 
		$description,
		$code = ""){
		
		$cn = Database::getConnection();
		try {
			$sql = <<<SQL
insert into erp_accounts (client_id, name, description, code, created)
values (:client_id, :name, :description, :code, :created);
SQL;

			$stmt = $cn->prepare($sql);
			if($stmt){
				if($stmt->bind_param(
					"isssi",
					$client_id,
					$name,
					$description,
					$code,
					REQUEST_TIME
				)){
					if($stmt->execute()){
						$id = $cn->insert_id();
						$sql = <<<SQL
insert into account_balances (account_id, client_id, updated)
values (:account_id, :client_id, :updated)
SQL;
						$stmt = $cn->prepare($sql);
						if($stmt){
							if($stmt->bind_param(
								"iii",
								$id,
								$client_id,
								REQUEST_TIME
							)){
								if($stmt->execute()){
									return $id;
								} else {
									throw new Exception("Unable to add account balance");
								}
							} else {
								throw new Exception("Unable to bind parameters to sql command to add account balance");
							}
						} else {
							throw new Exception("Unable to prepare sql command to add account balance");
						}
					} else {
						throw new Exception("Unable to add account");
					}
				} else {
					throw new Exception("Unable to bind parameters to sql command to add account");
				}
			} else {
				throw new Exception("Unable to prepare sql command to add account");
			}
			
		}
		
	}
	
}