<?php
function erp_account_accounts_config_form($form, $form_state){
	
	$form["actions"]["save"] = array(
		"#type"=>"submit",
		"#value"=>t("Save"),
		"#name"=>"btn-name"
	);
	
	return $form;
	
}

function erp_account_accounts_config_form_validate($form, &$form_state){

}

function erp_account_accounts_config_form_submit($form, &$form_state){

}