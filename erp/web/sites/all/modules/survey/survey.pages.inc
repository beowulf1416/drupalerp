<?php
function survey_form($form, $form_state, $survey = null){

	$form["survey"] = array(
		"#type"=>"value",
		"#value"=>$survey
	);
	
	$form["page"] = array(
		"#tree"=>true
	);
	
	return $form;
	
}

function survey_form_validate($form, &$form_state){

}

function survey_form_submit($form, &$form_state){

}

function survey_edit_form($form, $form_state, $survey = null){
	
	$form["survey"] = array(
		"#type"=>"value",
		"#value"=>$survey
	);
	
	$form["use-pages"] = array(
		"#type"=>"checkbox",
		"#title"=>t("Use pages (wizard)"),
		"#required"=>true,
		"#default_value"=>true
	);
	
	if(!is_null($survey) && isset($form_state["values"]["user-pages"])){
		$form["link-survey-pages-add"] = array(
			"#markup"=>l("Add survey page","survey/{$survey->id}/page/add")
		);
	}
	
	$options = array();
	$form["questions"] = array(
		"#type"=>"tableselect",
		"#header"=>array(
			"page_id"=>t("Page"),
			"text"=>t("Text"),
			"type"=>t("Type")
		),
		"#options"=>$options
	);
	if(!is_null($survey)){
		$form["link-survey-questions-add"] = array(
			"#markup"=>l("Add question","survey/{$survey->id}/question/add")
		);
	}
	
	if(!is_null($survey)){
		$form["closed"] = array(
			"#type"=>"checkbox",
			"#title"=>t("This survey is closed"),
			"#required"=>false,
			"#default_value"=>false
		);
	}
	
	return $form;
	
}

function survey_edit_form_validate($form, &$form_state){

}

function survey_edit_form_submit($form, &$form_state){

}

function survey_page_form($form, $form_state, $survey = null, $page = null){
	
}

function survey_page_form_validate($form, &$form_state){

}

function survey_page_form_submit($form, &$form_state){

}

function survey_question_form($form, $form_state, $survey = null, $question = null){
	
	$form["survey"] = array(
		"#type"=>"value",
		"#value"=>$survey
	);
	
	$form["question"] = array(
		"#type"=>"value",
		"#value"=>$question
	);
	
	$form["name"] = array(
		"#type"=>"textfield",
		"#title"=>t("Name"),
		"#required"=>true,
		"#default_value"=>""
	);

	$types = array();
	$form["type"] = array(
		"#type"=>"select",
		"#title"=>t("Type"),
		"#required"=>true,
		"#options"=>$options,
		"#default_value"=>0
	);
	
	$form["actions"]["cancel"] = array(
		"#type"=>"submit",
		"#name"=>"btn-cancel",
		"#value"=>"Cancel"
	);
	
	$form["actions"]["save"] = array(
		"#type"=>"submit",
		"#name"=>"btn-save",
		"#value"=>"Save"
	);
	
	return $form;
	
}

function survey_question_form_validate($form, &$form_state){
	
}

function survey_question_form_submit($form, &$form_state){

}