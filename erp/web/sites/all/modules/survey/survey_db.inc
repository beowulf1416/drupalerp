<?php
class SurveyDb {
	
	public static function post_install(){
		
		$values = array(
				array("id"=>QTYPE_TEXT,"name"=>"text"),
				array("id"=>QTYPE_YES_NO,"name"=>"yes/no"),
				array("id"=>QTYPE_TRUE_FALSE,"name"=>"true/false"),
				array("id"=>QTYPE_SELECT_SINGLE,"name"=>"single-select"),
				array("id"=>QTYPE_SELECT_MULTI,"name"=>"multi-select")
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
			->execute();
		$results = $qry->fetchAll(PDO::FETCH_ASSOC);
		
		$return = array();
		foreach($results as $r){
			$return[$r["id"]] = $r;
		}
		
		return $return;
		
	}
	
	public static function survey_page_add($survey_id, $title, $weight){
		
		return db_insert("survey_pages")
			->fields(array(
				"survey_id"=>$survey_id,
				"title"=>$title,
				"weight"=>$weight
			))
			->execute();
		
	}
	
	public static function survey_question_add($survey_id, $name, $text, $type_id, $page_id, $weight){
		
		return db_insert("survey_questions")
			->fields(array(
				"survey_id"=>$survey_id,
				"page_id"=>$page_id,
				"type_id"=>$type_id,
				"weight"=>$weight,
				"name"=>$name,
				"text"=>$text
			))
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
	
	public static function get_questions($survey_id){
		
		$results = db_select("survey_questions","a")
			->fields("a")
			->condition("survey_id", $survey_id, "=")
			->execute()
			->fetchAll(PDO::FETCH_ASSOC);
		return $results;
		
	}
	
	public static function get_pages($survey_id){
		
		$qry = db_select("survey_pages","a")
			->fields("a")
			->execute();
		$results = $qry->fetchAll(PDO::FETCH_ASSOC);
		return $results;
		
	}
	
}