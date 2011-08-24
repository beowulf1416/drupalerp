<?php
class AccountEntityController extends DrupalDefaultEntityController {
	
	function buildQuery($ids, $conditions = array(), $revision_id = FALSE){
		
		$query = parent::buildQuery($ids, $conditions, $revision_id);
		$query->leftJoin("erp_account_hiearchy", "eah", "%alias.id = eah.parent_acct_id");
		$query->fields("eah", array("parent_acct_id", "account_id"));
		
		return $query;
		
	}
	
}