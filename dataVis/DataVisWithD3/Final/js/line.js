class Plot {
	constructor(data, currentState) {
		this.margin = { top: 20, right: 20, bottom: 60, left: 80 };
        this.width = 810 - this.margin.left - this.margin.right;
        this.height = 500 - this.margin.top - this.margin.bottom;
        this.data = data;
        this.state = currentState;
        
        this.drawPlot();
        }
	drawPlot() {
		d3.select('#line-chart')
            .append('div').attr('id', 'chart-view');

        d3.select('#chart-view')
            .append('div')
            .attr("class", "tooltip")
            .style("opacity", 0);

        d3.select('#chart-view')
            .append('svg').classed('plot-svg', true)
            .attr("width", this.width + this.margin.left + this.margin.right)
            .attr("height", this.height + this.margin.top + this.margin.bottom);

        this.svgGroup = d3.select('#chart-view').select('.plot-svg').append('g').classed('wrapper-group', true);

        this.svgGroup.append('g').attr('id', 'xAxis');
        this.svgGroup.append('g').attr('id', 'yAxis');

        let dropdownWrap = d3.select('#chart-view').append('div').classed('dropdown-wrapper', true);

        let stateWrap = dropdownWrap.append("div").classed("dropdown-panel", true);

        stateWrap.append("div").classed("state-label", true)
        	     .append("text").text("State");

       	stateWrap.append("div").attr("id", "dropdown_state").classed("dropdown", true).append("div").classed("dropdown-content", true)
       			 .append("select");
    
        this.drawDropDown();
        this.updatePlot("Washington");
	}
	updatePlot(state) {
		let filteredData = [];
		let maxCases = -Infinity;
		this.data.forEach(function(d){
			if (d.state == state){
				filteredData.push(d);
				if (parseInt(d.cases) > maxCases) {
					maxCases = parseInt(d.cases); 
				}
			}
		});

		let startDate = new Date(2020, 0, 22);
		let endDate = new Date(2020, 5, 7);

		var yScale = d3.scaleLinear().domain([0, maxCases]).range([this.height - this.margin.top, 0])
		var xScale = d3.scaleTime().domain([startDate, endDate]).range([0, this.width]);

		let xAxis = d3.axisBottom().scale(xScale);
		let currxAxis = this.svgGroup.select("#xAxis");
		currxAxis.exit().remove();
		let newXAxis = this.svgGroup.enter().append('g');
		currxAxis = currxAxis.merge(newXAxis); 
		currxAxis.call(xAxis).classed("axis", true)
                     .attr("transform", "translate(" + this.margin.left + "," + this.height + ")");
        currxAxis.selectAll(".tick").classed("axis-label", true);

        let yAxis = d3.axisLeft().scale(yScale);
        let curryAxis = this.svgGroup.select("#yAxis");
        curryAxis.exit().remove();

        let newyAxis = this.svgGroup.enter().append("g");
        curryAxis = curryAxis.merge(newyAxis);
        curryAxis.call(yAxis).classed("axis",true)
                     .attr("transform", "translate(" + this.margin.left + "," + this.margin.top + ")")
        curryAxis.selectAll(".tick").classed("axis-label", true);

        let curCircles = this.svgGroup.selectAll("circle").data(filteredData);
        curCircles.exit().remove();

        let tempCircles = curCircles.enter().append("circle");
        curCircles = curCircles.merge(tempCircles)

        let newCircles = curCircles.attr("class", "plottedCases")
        						   .transition().duration(500)
        						   .attr("cx", function(d){
        						   		  let dA = d.date.split("-");
					   				      let year = parseInt(dA[0]);
					   				 	  let month = dA[1].substr(1);
					   				 	  let intMonth = parseInt(month) - 1;
					   				 	  let day = parseInt(dA[2]);
					   				 	  let newDate = new Date(year, intMonth, day);
					   				 	  return 80 + xScale(newDate);
        						   })
        						   .attr("cy", function(d){
        						   		return 20 + yScale(parseInt(d.cases));;
        						   })
        						   .attr("r", 1.5)
        						   .attr("fill", "red");						  
	}

	drawDropDown(){
		let that = this;
		let dropDownWrapper = d3.select(".dropdown-wrapper");
		let dropData = [];

		this.data.forEach(function(d) {
			if (!dropData.includes(d.state)) {
				dropData.push(d.state)
			}
		});

		dropData = dropData.sort()
		let stateDrop = dropDownWrapper.select("#dropdown_state").select(".dropdown-content").select("select");
		
		let stateOptions = stateDrop.selectAll("option").data(dropData);

		let tempOptions = stateOptions.enter().append("option")
			.attr("value", (d,i) => d);

		tempOptions.append("text").text((d,i) => d);

		stateOptions = tempOptions.merge(stateOptions); 

		stateDrop.on("change", function(d, i) {
			that.state = this.options[this.selectedIndex].value;
			that.updatePlot(that.state);
		});
	}
}