<?php
class SurveyDb {
	
	public static function post_install(){
		
		$values = array(
				array("id"=>QTYPE_TEXT,"name"=>"text"),
				array("id"=>QTYPE_YES_NO,"name"=>"yes/no"),
				array("id"=>QTYPE_TRUE_FALSE,"name"=>"true/false"),
				array("id"=>QTYPE_SELECT_SINGLE,"name"=>"single-select"),
				array("id"=>QTYPE_SELECT_MULTI,"name"=>"multi-select"),
				array("id"=>QTYPE_NUMBER,"name"=>"number"),
				array("id"=>QTYPE_DATE,"name"=>"date"),
				array("id"=>QTYPE_REGEX,"name"=>"regex")
			);
		
		$qry = db_insert("survey_question_types")
			->fields(array(
				"id",
				"name"
			));
		foreach($values as $v){
			$qry->values($v);
		}
		$results = $qry->execute();
		
	}
	
	public static function survey_get_all(){
		
		$qry = db_select("surveys","a")
			->fields("a")
			->isNull("a.closed")
 			->execute();
		
		$results = $qry->fetchAll(PDO::FETCH_ASSOC);
		return $results;
		
	}
	
	public static function survey_add($title, $desc){
		
		return db_insert("surveys")
			->fields(array(
				"title"=>$title,
				"description"=>$desc,
				"created"=>REQUEST_TIME
			))
			->execute();
		
	}
	
	public static function survey_page_get_all($survey_id){
		
		$qry = db_select("survey_pages","a")
			->fields("a")
			->condition("a.survey_id", $survey_id, "=")
			->orderBy("a.weight")
			->execute();
		$results = $qry->fetchAll(PDO::FETCH_ASSOC);
		
		$return = array();
		foreach($results as $r){
			$return[$r["id"]] = $r;
		}
		
		return $return;
		
	}
	
	public static function survey_page_add($survey_id, $title, $description, $weight){
		
		return db_insert("survey_pages")
			->fields(array(
				"survey_id"=>$survey_id,
				"title"=>$title,
				"description"=>$description,
				"weight"=>$weight
			))
			->execute();
		
	}
	
	public static function survey_question_add($survey_id, $name, $text, $description, $type_id, $page_id, $weight, $required){
		
		return db_insert("survey_questions")
			->fields(array(
				"survey_id"=>$survey_id,
				"page_id"=>$page_id,
				"type_id"=>$type_id,
				"weight"=>$weight,
				"name"=>$name,
				"text"=>$text,
				"description"=>$description,
				"is_required"=>$required
			))
			->execute();
		
	}
	
	public static function survey_question_update($question_id, $name, $text, $description, $type_id, $page_id, $weight, $required){
		
		db_update("survey_questions")
			->fields(array(
				"text"=>$name,
				"description"=>$description,
				"type_id"=>$type_id,
				"page_id"=>$page_id,
				"weight"=>$weight,
				"is_required"=>$required
			))
			->condition("id", $question_id, "=")
			->execute();
		
	}
	
	public static function get_question_types(){
		
		$qry = db_select("survey_question_types","a")
			->fields("a")
			->execute();
		$results = $qry->fetchAll(PDO::FETCH_ASSOC);
		
		$return = array();
		foreach($results as $r){
			$return[$r["id"]] = $r;
		}
		
		return $return;
		
	}
	
	public static function get_questions($survey_id, $page_id = null){
		
		$qry = db_select("survey_questions","a")
			->fields("a")
			->condition("a.survey_id", $survey_id, "=")
			->orderBy("a.weight");
		
		if(isset($page_id)){
			$qry->condition("a.page_id", $page_id, "=")
				->orderBy("a.page_id");
		}
		
		
		$results = $qry->execute()
			->fetchAll(PDO::FETCH_ASSOC);
		
		return $results;
		
	}
	
	public static function get_pages($survey_id){
		
		$qry = db_select("survey_pages","a")
			->fields("a")
			->condition("a.survey_id", $survey_id, "=")
			->orderBy("a.weight")
			->execute();
		$results = $qry->fetchAll(PDO::FETCH_ASSOC);
		return $results;
		
	}
	
	public static function option_get_all($survey_id, $question_id){
		
		$qry = db_select("survey_question_options","a")
			->fields("a")
			->condition("a.survey_id", $survey_id, "=")
			->condition("a.question_id", $question_id, "=")
			->execute();
		$results = $qry->fetchAll(PDO::FETCH_ASSOC);
		return $results;
		
	}
	
	public static function option_add($survey_id, $question_id, $title, $value){
		
		return db_insert("survey_question_options")
			->fields(array(
				"survey_id"=>$survey_id,
				"question_id"=>$question_id,
				"title"=>$title,
				"value"=>$value
			))
			->execute();
		
	}
	
	public static function option_update($option_id, $title, $value){
		
		db_update("survey_question_options")
			->fields(array(
				"title"=>$title,
				"value"=>$value
			))
			->condition("id", $option_id, "=")
			->execute();
		
	}
	
}