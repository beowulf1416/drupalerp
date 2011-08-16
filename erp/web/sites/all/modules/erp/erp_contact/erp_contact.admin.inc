<?php

function erp_contact_config_form($form, $form_state){
	
	$form["actions"]["save"] = array(
		"#type"=>"submit",
		"#value"=>t("Save"),
		"#name"=>"btn-name"
	);
	
	$form["actions"]["cancel"] = array(
		"#type"=>"submit",
		"#value"=>t("Cancel"),
		"#name"=>"btn-cancel"
	);
	
	return $form;
	
}

function erp_contact_config_form_validate($form, &$form_state){
	
}

function erp_contact_config_form_submit($form, &$form_state){
	
}