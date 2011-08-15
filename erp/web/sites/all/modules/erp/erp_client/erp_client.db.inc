<?php
class ClientDB {
	
	private $_cn;
	
	public function __construct(){
		$this->_cn = Database::getConnection();
	}
	
	public function create_tables(){

		$type = $this->_cn->databaseType();
		if(file_exists("db\{$type}.sql")){
			$sql = file_get_contents("db\{$type}.sql");
			
			$stmt = $this->_cn->prepare($sql);
			$stmt->execute();
			
		} else {
			throw new Exception("Unable to determine database type");
		}
		
	}
	
}