<?php
/**
 * 
 * Enter description here ...
 * @author ftomale
 *
 */
class ContactDb {
	
	public static function modify_tables(){
		
		$cn = Database::getConnection();
		$type = $cn->databaseType();
		$path = DRUPAL_ROOT . "/" . drupal_get_path("module","erp_contact");
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
	
	public static function contact_add(
		$last_name, 
		$first_name, 
		$middle_name, 
		$prefix, 
		$suffix,
		$client_id,
		$uid){
		
		return db_insert("erp_contacts")
			->fields(array(
				"last_name"=>$last_name,
				"first_name"=>$first_name,
				"middle_name"=>$middle_name,
				"prefix"=>$prefix,
				"suffix"=>$suffix,
				"client_id"=>$client_id,
				"uid"=>$uid
			))
			->execute();
		
	}
	
	public static function contact_remove($contact_id){
		
		db_delete("erp_contacts")
		->condition("id",$contact_id,"=")
		->execute();
		
	}
	
	public static function contact_update(
		$contact_id,
		$last_name,
		$first_name,
		$middle_name,
		$prefix,
		$suffix,
		$client_id,
		$uid){
		
		db_update("erp_contacts")
		->fields(array(
			"last_name"=>$last_name,
			"first_name"=>$first_name,
			"middle_name"=>$middle_name,
			"prefix"=>$prefix,
			"suffix"=>$suffix,
			"client_id"=>$client_id,
			"uid"=>$uid
		))
		->condition("id", $id, "=")
		->execute();
		
	}
	
}