loadData().then(data => {
	this.activeState = "Alabama";
	this.activeDate = "2020-06-07"
	let that = this;


	function updateDate(date){
		that.activeDate = date;
		console.log("update date called")
	}

	const usMap = new Map(data);
	const linePlot = new Plot(data, this.activeState);
	
	function updateState(date) {
		//TODO: delete it first
		// d3.json("data/us-states.json").then(mapData => {
		// 	usMap.drawMap(mapData, date); 
		// })
		console.log("update state called")
	}

	d3.json("data/us-states.json").then(mapData => {
			usMap.drawMap(mapData, this.activeDate); 
		})
});


// load data and store json objects in an array
async function loadData() {
	let data = await d3.csv("data/us-states.csv");
	let dataMap = [];
	data.forEach(function(row) {
		let dataPiece = {};
		dataPiece["date"] = row.date;
		dataPiece["state"] = row.state;
		dataPiece["cases"] = row.cases;
		dataPiece["deaths"] = row.deaths;
		dataMap.push(dataPiece); 
	});
	// console.log(dataMap);
	return dataMap;
}