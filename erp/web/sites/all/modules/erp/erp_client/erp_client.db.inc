<?php
class ClientDB {
	
	private $_cn;
	
	public function __construct(){
		$this->_cn = Database::getConnection();
	}
	
	public function modify_tables(){

		$type = $this->_cn->databaseType();
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
	
}