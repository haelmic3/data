onmessage=function(e)
{
	let
		file=e.data,
		reader = new FileReaderSync(),
		result = reader.readAsText(file),
	_;
	postMessage(result);
}
onmessageerror=function(e)
{
	postMessage(e.data);
}
onerror=function(e)
{
	postMessage(e.data);
}
