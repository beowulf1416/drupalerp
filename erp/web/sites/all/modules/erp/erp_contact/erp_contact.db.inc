<?php
/**
 * 
 * Enter description here ...
 * @author ftomale
 *
 */
class ContactDb {
	
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
	
}