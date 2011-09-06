<?php
class SurveyDb {
	
	public static function survey_get_all(){
		
		$qry = db_select("surveys","a")
			->fields("a")
			->isNull("a.closed")
			->execute();
		$results = $qry->fetchAll(PDO::FETCH_ASSOC);
		return $results;
		
	}
	
	public static function survey_add($title, $desc){
		
		db_insert("surveys")
			->fields(array(
				"title"=>$title,
				"description"=>$desc,
				"created"=>REQUEST_TIME
			))
			->execute();
		
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
		
		
		
	}
	
}