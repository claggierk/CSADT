<?php
	//http://www.w3schools.com/php/php_file_upload.asp
	//http://www.developphp.com/view_lesson.php?v=449
	//http://www.php.net/manual/en/function.is-uploaded-file.php
	
	function displayHelp($fileName, $allowedFileTypes)
	{ 
	   echo "Invalid file     : " . $fileName . "<br>";
	   echo "Valid file types : <br>";
	   print_r($allowedFileTypes);
	}
	
	$fileName = $_FILES["file"]["name"];
	$tempFileName = $_FILES["file"]["tmp_name"];
	$allowedFileTypes = array("txt", "xlsx", "jpg");
	$temp = explode(".", $fileName);
	$fileText = reset($temp);
	$extension = end($temp);
	
	$output = "output/";

	if(in_array($extension, $allowedFileTypes))
	{
		if($_FILES["file"]["error"] > 0)
		{
			echo "Return Code: " . $_FILES["file"]["error"] . "<br>";
		}
		else
		{
			echo "Upload: " . $fileName . "<br>";
			echo "Type: " . $_FILES["file"]["type"] . "<br>";
			echo "Size: " . ($_FILES["file"]["size"] / 1024) . " kB<br>";
			
			if(!is_uploaded_file($tempFileName))
			{
				echo "ERROR: File \"" . $fileName . "\" not uploaded<br>";
			}
			
			$uploadFilePath = "uploads/" . $fileName;
			if(file_exists($uploadFilePath))
			{
				echo $uploadFilePath . " already exists... overwriting<br>";
			}
			
			$moveResult = move_uploaded_file($tempFileName, $uploadFilePath);
			if($moveResult == false)
			{
				 echo "ERROR: File\"" . $tempFileName. "\" failed to move to \"" . $uploadFilePath . "\"<br>";
			}
			if(!file_exists($uploadFilePath))
			{
				echo "ERROR: " . $uploadFilePath . " does not exist...";
			}
			echo "File Stored: " . $uploadFilePath . "<br>";
		}
		
		$preprocessedFileName = $fileText . "_Preprocessed.txt";
		$preprocessedFilePath = $output . $preprocessedFileName;
		exec("python scripts/2_TextToPreprocessedText.py " . $uploadFilePath . " " . $preprocessedFilePath);
		echo "<a href=" . $uploadFilePath . ">" . $fileName . "</a> <br>";
		echo "<a href=" . $preprocessedFilePath . ">" . $preprocessedFileName . "</a> <br>";
		
		$treeFile = $output . "Tree.txt";
		//exec("scripts/adt " . $preprocessedFilePath . " " . $treeFile);
	}
	else
	{
		displayHelp($fileName, $allowedFileTypes);
	}
?>