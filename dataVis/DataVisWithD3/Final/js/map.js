class Map {
	constructor(data, date) {
		this.projection = d3.geoAlbersUsa().scale(940).translate([365, 225]);
		this.margin = { top: 20, right: 20, bottom: 60, left: 80 };
		this.path = d3.geoPath().projection(this.projection);
		this.stateData = data;
	}

	drawMap(us, date) {

		d3.select("#map-chart")
		  .append("svg")
		  .attr("id", "mapsvg");

		d3.select("#map-chart")
			  .append("div").attr("id", "activeDate-bar");

		this.totalCases = d3.select("#text-box")
		  					.append("div").attr("id", "Total-Cases");

		this.totalCases.append("text")
		 			   .attr("x", 0)
		 			   .attr("y", 200)
		 			   .html("Total Cases: ")
		this.totalDeaths = d3.select("#text-box")
		  				     .append("div").attr("id", "Total-Deaths");
		this.totalDeaths.append("text")
						.attr("x", 0)
						.attr("y", 500)
						.html("Total Deaths: ")

		d3.select("#mapsvg").selectAll("path")
		  .data(us.features)
		  .join("path")
		  .attr("d", this.path)
		  .append("svg:title");

		  this.updateMap(us, date)
		  this.drawDateSlider(us, date);
	}
	updateMap(us, date){
		let filteredData = [];
		this.stateData.forEach(function(entry){
			if (entry.date) {
				if (entry.date == date) {
					filteredData.push(entry);
				}
			}
		});
		let cases = this.stateData.map(function(d){
			return d.cases
		})
		let totalCases = 0;
		let totalDeaths = 0; 
		let dataLookup = {};
				filteredData.forEach(function (stateRow) {
					dataLookup[stateRow.state] = parseInt(stateRow.cases);
					totalCases += parseInt(stateRow.cases);
					totalDeaths += parseInt(stateRow.deaths); 
				}); 

				us.features.forEach(function (feature) {
					feature.properties.cases = dataLookup[feature.properties.name];
				});
		let totalCasesStr = "Total Cases: " + totalCases.toString().replace(/\B(?=(\d{3})+(?!\d))/g, ",");
		let totalDeathsStr = "Total Deaths: " + totalDeaths.toString().replace(/\B(?=(\d{3})+(?!\d))/g, ",");

		this.totalCases.html(totalCasesStr);
		this.totalDeaths.html(totalDeathsStr);

		let colorScale = d3.scaleSequential()
			.domain([d3.min(cases), d3.max(cases)])
			.interpolator(d3.interpolate("rgb(108,99,255)", "red"));

		d3.select("#mapsvg").selectAll("path")
		  .data(us.features)
		  .join("path")
		  .attr("id", function(d) {
		  	return d.properties.cases;
		  })
		  .attr("d", this.path)
		  .style("fill", function(d) {
		  	if (d.properties.cases == undefined){
		  		return 0;
		  	}
		  	else {
		  		return colorScale(d.properties.cases);
		  	}
		  })
		  .select("title").text(function(d){
		  	if (d.properties.cases == undefined) {
		  		return 0;
		  	}
		  	else {
		  		return "State: " + d.properties.name + "\nNumber of Cases: " + d.properties.cases;
		  	}
		  });
	}
	drawDateSlider(us, date){
		let that = this;
		let dateArray = [];
		this.stateData.forEach(function(d){
			if (!dateArray.includes(d.date)){
				dateArray.push(d.date);
			}
		});

		let dateIndex = dateArray.indexOf(date);
		let dateScale = d3.scaleLinear().domain(0, dateArray.length - 1).range([30, 730]);
		let dateSlider = d3.select("#activeDate-bar")
			.append("div").classed("slider-wrap", true)
			.append("input").classed("slider", true)
			.attr("type", "range")
			.attr("min", 0)
			.attr("max", dateArray.length - 1)
			.attr("value", dateIndex); 

		let sliderLabel = d3.select(".slider-wrap")
			.append("div").classed("slider-label", true)
			.append("svg");

		let sliderText = sliderLabel.append("text").text(date);
		sliderText.attr("x", 50);
		sliderText.attr("y", 25);

		dateSlider.on("input", function(){
			let curDate = dateSlider.node().value;
			sliderText.text(dateArray[curDate]);
			that.updateMap(us, dateArray[curDate]);
		});
	}
}