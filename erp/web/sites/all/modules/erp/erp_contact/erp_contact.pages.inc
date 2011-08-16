<?php
function erp_contact_form($form, $form_state, $client=null, $contact=null){
	
	$form["name-first"] = array(
		"#type"=>"textfield",
		"#title"=>t("First name"),
		"#required"=>true,
		"#maxlength"=>100
	);
	
	$form["name-last"] = array(
		"#type"=>"textfield",
		"#title"=>t("Last name"),
		"#required"=>true,
		"#maxlength"=>100
	);
	
	$form["name-middle"] = array(
		"#type"=>"textfield",
		"#title"=>t("Middle"),
		"#required"=>false,
		"#maxlength"=>100
	);
	
	$form["name-prefix"] = array(
		"#type"=>"textfield",
		"#title"=>t("Prefix"),
		"#required"=>false,
		"#maxlength"=>50
	);
	
	$form["name-suffix"] = array(
		"#type"=>"textfield",
		"#title"=>t("Sufffix"),
		"#required"=>false,
		"#maxlength"=>50
	);
	
	$form["actions"]["save"] = array(
		"#type"=>"submit",
		"#value"=>t("Save"),
		"#name"=>"btn-save"
	);
	
	$form["actions"]["cancel"] = array(
		"#type"=>"submit",
		"#value"=>t("Cancel"),
		"#name"=>"btn-cancel"
	);
	
	return $form;
	
}

function erp_contact_form_validate($form, &$form_state){

}

function erp_contact_form_submit($form, &$form_state){

}

function erp_contact_delete_form($form, $form_state, $client=null, $contact=null){
	
	$form["msg"] = array(
		"#markup"=>"<div class=\"form-msg-warning\">".
			t("Are you sure you want to delete %contact?",
				array("%contact"=>"//TODO")).
				"</div><!-- .form-msg-warning -->"
	);
	
	$form["actions"]["yes"] = array(
		"#type"=>"submit",
		"#value"=>t("Yes"),
		"#name"=>"btn-yes"
	);
	
	$form["actions"]["no"] = array(
		"#type"=>"submit",
		"#value"=>t("No"),
		"#name"=>"btn-no"
	);
	
	return $form;
	
}

function erp_contact_delete_form_validate($form, &$form_state){

}

function erp_contact_delete_form_submit($form, &$form_state){

}