<?php
class ClientDB {
	
	public static function modify_tables(){

		$cn = Database::getConnection();
		$type = $cn->databaseType();
		$path = DRUPAL_ROOT . "/" . drupal_get_path("module","erp_client");
		if(file_exists("{$path}/db/{$type}.sql")){
			$sql = file_get_contents("{$path}/db/{$type}.sql");
			if($sql != ""){			
				$stmt = $this->_cn->prepare($sql);
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
	
	public static function add_client($name, $desc){
		
		return db_insert("erp_clients")
			->fields(array(
				"name"=>$name,
				"description"=>$desc,
				"created"=>REQUEST_TIME
			))
			->execute();
		
	}
	
	public static function get_client($client_id){
		
		return db_select("erp_clients","a")
			->fields("a")
			->condition("id",$client_id,"=")
			->execute()
			->fetchAll(PDO::FETCH_ASSOC);
		
	}
	
	public static function toggle_client_status($client_id, $enable){
		
		db_update("erp_clients")
		->fields(array(
			"enabled"=>$enable
		))
		->condition("id", $client_id, "=")
		->execute();
		
	}
}