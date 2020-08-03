/** Data structure for the data associated with an individual country. */
class PlotData {
    /**
     *
     * @param country country name from the x data object
     * @param xVal value from the data object chosen for x at the active year
     * @param yVal value from the data object chosen for y at the active year
     * @param id country id
     * @param region country region
     * @param circleSize value for r from data object chosen for circleSizeIndicator
     */
    constructor(country, xVal, yVal, id, region, circleSize) {
        this.country = country;
        this.xVal = xVal;
        this.yVal = yVal;
        this.id = id;
        this.region = region;
        this.circleSize = circleSize;

    }
}

/** Class representing the scatter plot view. */
class GapPlot {

    /**
     * Creates an new GapPlot Object
     *
     * For part 2 of the homework, you only need to worry about the first parameter.
     * You will be updating the plot with the data in updatePlot,
     * but first you need to draw the plot structure that you will be updating.
     *
     * Set the data as a variable that will be accessible to you in updatePlot()
     * Call the drawplot() function after you set it up to draw the plot structure on GapPlot load
     *
     * We have provided the dimensions for you!
     *
     * @param updateCountry a callback function used to notify other parts of the program when the selected
     * country was updated (clicked)
     * @param updateYear a callback function used to notify other parts of the program when a year was updated
     * @param activeYear the year for which the data should be drawn initially
     */
    constructor(data, updateCountry, updateYear, activeYear) {

        // ******* TODO: PART 2 *******

        this.margin = { top: 20, right: 20, bottom: 60, left: 80 };
        this.width = 810 - this.margin.left - this.margin.right;
        this.height = 500 - this.margin.top - this.margin.bottom;
        this.activeYear = activeYear;

        this.data = data;

        //YOUR CODE HERE  

        this.updateYear = updateYear;
        this.updateCountry = updateCountry;

        this.dataRangeStore = {
                            'population':      {'min': Infinity, 'max': -Infinity},
                            'gdp':             {'min': Infinity, 'max': -Infinity},
                            'child-mortality': {'min': Infinity, 'max': -Infinity},
                            'life-expectancy': {'min': Infinity, 'max': -Infinity},
                            'fertility-rate':  {'min': Infinity, 'max': -Infinity}
        }
        this.drawPlot();
        
        // ******* TODO: PART 3 *******
        /**
         For part 4 of the homework, you will be using the other 3 parameters.
         * assign the highlightUpdate function as a variable that will be accessible to you in updatePlot()
         * assign the dragUpdate function as a variable that will be accessible to you in updatePlot()
         */

        //YOUR CODE HERE  


    }

    /**
     * Sets up the plot, axes, and slider,
     */

    drawPlot() {
        // ******* TODO: PART 2 *******
        /**
         You will be setting up the plot for the scatterplot.
         Here you will create axes for the x and y data that you will be selecting and calling in updatePlot
         (hint): class them.

         Main things you should set up here:
         1). Create the x and y axes
         2). Create the activeYear background text


         The dropdown menus have been created for you!

         */

        d3.select('#scatter-plot')
            .append('div').attr('id', 'chart-view');

        d3.select('#scatter-plot')
            .append('div').attr('id', 'activeYear-bar');

        d3.select('#chart-view')
            .append('div')
            .attr("class", "tooltip")
            .style("opacity", 0);

        d3.select('#chart-view')
            .append('svg').classed('plot-svg', true)
            .attr("width", this.width + this.margin.left + this.margin.right)
            .attr("height", this.height + this.margin.top + this.margin.bottom);

        //changing this so that it may be easily accessed/changed in other functions 
        this.svgGroup = d3.select('#chart-view').select('.plot-svg').append('g').classed('wrapper-group', true);

        //Defining axes here so they can be accessed in updatePlot
        this.svgGroup.append('g').attr('id', 'xAxis');
        this.svgGroup.append('g').attr('id', 'yAxis');
        //YOUR CODE HERE  
        let startYear = 1800;
        let endYear = 2020;

        //Changing to a class so that it may be updated by sliding the year bar
        this.activeYearBG = this.svgGroup.append("text")
                                   .attr("x", this.margin.left + 50)
                                   .attr("y", this.margin.top + 50)
                                   .html(this.activeYear)
                                   .classed("activeYear-background", true);

    

        /* This is the setup for the dropdown menu- no need to change this */
        let dropdownWrap = d3.select('#chart-view').append('div').classed('dropdown-wrapper', true);

        //Dropdown for circle
        let cWrap = dropdownWrap.append('div').classed('dropdown-panel', true);

        cWrap.append('div').classed('c-label', true)
            .append('text')
            .text('Circle Size');

        cWrap.append('div').attr('id', 'dropdown_c').classed('dropdown', true).append('div').classed('dropdown-content', true)
            .append('select');


        //Dropdown for X 
        let xWrap = dropdownWrap.append('div').classed('dropdown-panel', true);

        xWrap.append('div').classed('x-label', true)
            .append('text')
            .text('X Axis Data');

        xWrap.append('div').attr('id', 'dropdown_x').classed('dropdown', true).append('div').classed('dropdown-content', true)
            .append('select');

        //Dropdown for Y
        let yWrap = dropdownWrap.append('div').classed('dropdown-panel', true);

        yWrap.append('div').classed('y-label', true)
            .append('text')
            .text('Y Axis Data');

        yWrap.append('div').attr('id', 'dropdown_y').classed('dropdown', true).append('div').classed('dropdown-content', true)
            .append('select');

        d3.select('#chart-view')
            .append('div')
            .classed('circle-legend', true)
            .append('svg')
            .append('g')
            .attr('transform', 'translate(10, 0)');

        for (let obj of Object.keys(this.data)) { //iterate through keys of data
            let objData = this.data[obj]
            if (objData != undefined) {
                for (let country of objData){   //access country's data based using key
                    for (let i = startYear; i <= endYear; i++) { //iterate through the years and determine the min/max
                            if (this.dataRangeStore[obj].min > country[i]) {
                                this.dataRangeStore[obj].min = country[i]
                            }
                            else if (this.dataRangeStore[obj].max < country[i]){
                                this.dataRangeStore[obj].max = country[i];
                            }
                        }
                }
            }
        }



        console.log(this.dataRangeStore)
        this.drawDropDown("fertility-rate", "gdp", "population");
        this.updatePlot(2000,"fertility-rate", "gdp", "population")
        this.drawYearBar();
    }

    /**
     * Renders the plot for the parameters specified
     *
     * @param activeYear the year for which to render
     * @param xIndicator identifies the values to use for the x axis
     * @param yIndicator identifies the values to use for the y axis
     * @param circleSizeIndicator identifies the values to use for the circle size
     */
    updatePlot(activeYear, xIndicator, yIndicator, circleSizeIndicator) {

        // ******* TODO: PART 2 *******

        /*
        You will be updating the scatterplot from the data. hint: use the #chart-view div

        *** Structuring your PlotData objects ***
        You need to start by mapping the data specified by the parameters to the PlotData Object
        Your PlotData object is specified at the top of the file
        You will need get the data specified by the x, y and circle size parameters from the data passed
        to the GapPlot constructor

        *** Setting the scales for your x, y, and circle data ***
        For x and y data, you should get the overall max of the whole data set for that data category,
        not just for the activeYear.

        ***draw circles***
        draw the circles with a scaled area from the circle data, with cx from your x data and cy from y data
        You need to size the circles from your circleSize data, we have provided a function for you to do this
        called circleSizer. Use this when you assign the 'r' attribute.

        ***Tooltip for the bubbles***
        You need to assign a tooltip to appear on mouse-over of a country bubble to show the name of the country.
        We have provided the mouse-over for you, but you have to set it up
        Hint: you will need to call the tooltipRender function for this.

        *** call the drawLegend() and drawDropDown()
        These will draw the legend and the drop down menus in your data
        Pay attention to the parameters needed in each of the functions
        
        */

        /**
         *  Function to determine the circle radius by circle size
         *  This is the function to size your circles, you don't need to do anything to this
         *  but you will call it and pass the circle data as the parameter.
         * 
         * @param d the data value to encode
         * @returns {number} the radius
         */
        let minSize = this.dataRangeStore[circleSizeIndicator].min;
        let maxSize = this.dataRangeStore[circleSizeIndicator].max;

        let circleSizer = function(d) {
            let cScale = d3.scaleSqrt().range([3, 20]).domain([minSize, maxSize]);
            return d.circleSize ? cScale(d.circleSize) : 3;
        };
        ///////////////////////////////////////////////////////////////////
        //YOUR CODE HERE 
        let CountryData = []
        try {
            let countryGeos = this.data["population"].map(d => d.geo);

            for (let geo of countryGeos) {
                let xVal = this.data[xIndicator].find(d => d.geo == geo);
                if (xVal)
                    xVal = xVal[activeYear];
                else
                    xVal = undefined;
                let yVal = this.data[yIndicator].find(d => d.geo == geo);
                if (yVal)
                    yVal = yVal[activeYear];
                else 
                    yVal = undefined;
                let cVal = this.data[circleSizeIndicator].find(d => d.geo == geo);
                if (cVal)
                    cVal = cVal[activeYear];
                else
                    cVal = undefined;
                let region = this.data["population"].find(d => d.geo == geo);
                if (region)
                    region = region.region
                else 
                    region = "unknown"
                let country = this.data["gdp"].find(d => d.geo == geo);
                if (country)
                    country = country.country;
                else
                    country = "unknown"
                CountryData.push(new PlotData(country, xVal, yVal, geo, region, cVal));
            }
            let minX = this.dataRangeStore[xIndicator].min;
            let maxX = this.dataRangeStore[xIndicator].max;
            let xScale = d3.scaleLinear().domain([minX, maxX]).range([0, this.width]);

            let xAxis = d3.axisBottom().scale(xScale);
            let currxAxis = this.svgGroup.select("#xAxis")
            currxAxis.exit().remove(); 
            let newXAxis = this.svgGroup.enter().append('g');
            currxAxis = currxAxis.merge(newXAxis);

            currxAxis.call(xAxis).classed("axis", true)
                     .attr("transform", "translate(" + this.margin.left + "," + this.height + ")");
            currxAxis.selectAll(".tick").classed("axis-label", true); 

            let minY = this.dataRangeStore[yIndicator].min;
            let maxY = this.dataRangeStore[yIndicator].max;
            let yScale = d3.scaleLinear().domain([minY, maxY]).range([this.height - this.margin.top, 0]);
          
            let yAxis = d3.axisLeft().scale(yScale);
            let curryAxis = this.svgGroup.select("#yAxis");
            curryAxis.exit().remove();

            let newyAxis = this.svgGroup.enter().append('g');
            curryAxis = curryAxis.merge(newyAxis);

            curryAxis.call(yAxis).classed("axis",true)
                     .attr("transform", "translate(" + this.margin.left + "," + this.margin.top + ")")
            curryAxis.selectAll(".tick").classed("axis-label", true);

        this.svgGroup.append("text").attr("id", "xAxisLabel");
        this.svgGroup.append("text").attr("id", "yAxisLabel");

        let xLabel = d3.select("#dropdown_x").select(".dropdown-content").select("select").node()
        let yLabel = d3.select("#dropdown_y").select(".dropdown-content").select("select").node()

        let xText = xLabel[xLabel.selectedIndex].text;
        let yText = yLabel[yLabel.selectedIndex].text;
""
        let curXLabel = this.svgGroup.select("#xAxisLabel");
        curXLabel.exit().remove();

        let tempXLabel = curXLabel.enter().append("text");
        curXLabel = curXLabel.merge(tempXLabel);
        curXLabel.datum(xText).classed("x-label", true)
                 .attr("transform", "translate(" + (0.5*this.width) + "," + (this.height + this.margin.top * 1.75) + ")")
                 .text(d => d.toUpperCase());

        let curYLabel = this.svgGroup.select("#yAxisLabel");
        curYLabel.exit().remove();

        let tempYLabel = curYLabel.enter().append("text");
        curYLabel = curYLabel.merge(tempYLabel);
        curYLabel.datum(yText).classed("y-label", true)
                 .attr("transform", " translate(15, 275) rotate(-90)")
                 .text(d => d.toUpperCase());

        let curCircles = this.svgGroup.selectAll("circle").data(CountryData);
        curCircles.exit().remove();

        let tempCircles = curCircles.enter().append("circle");
        curCircles = curCircles.merge(tempCircles)

        let newCircles = curCircles.attr("class", d => "circle " + d.region)
                                   .attr("cx", d => this.margin.left + xScale(d.xVal))
                                   .attr("cy", d => this.margin.top  + yScale(d.yVal))
                                   .attr('r', circleSizer)
                                   .attr("id", d => (d.id.toUpperCase())).append("title")
                                   .html(this.tooltipRender);



        this.drawDropDown(xIndicator, yIndicator, circleSizeIndicator);
        this.drawLegend(minSize, maxSize);

        }
        catch(error){
            console.log(error)
        }
    }

    /**
     * Setting up the drop-downs
     * @param xIndicator identifies the values to use for the x axis
     * @param yIndicator identifies the values to use for the y axis
     * @param circleSizeIndicator identifies the values to use for the circle size
     */
    drawDropDown(xIndicator, yIndicator, circleSizeIndicator) {

        let that = this;
        let dropDownWrapper = d3.select('.dropdown-wrapper');
        let dropData = [];

        for (let key in this.data) {
            dropData.push({
                indicator: key,
                indicator_name: this.data[key][0].indicator_name
            });
        }
        console.log(dropData);
        /* CIRCLE DROPDOWN */
        let dropC = dropDownWrapper.select('#dropdown_c').select('.dropdown-content').select('select');

        let optionsC = dropC.selectAll('option')
            .data(dropData);


        optionsC.exit().remove();

        let optionsCEnter = optionsC.enter()
            .append('option')
            .attr('value', (d, i) => d.indicator);

        optionsCEnter.append('text')
            .text((d, i) => d.indicator_name);

        optionsC = optionsCEnter.merge(optionsC);

        let selectedC = optionsC.filter(d => d.indicator === circleSizeIndicator)
            .attr('selected', true);

        dropC.on('change', function(d, i) {
            let cValue = this.options[this.selectedIndex].value;
            let xValue = dropX.node().value;
            let yValue = dropY.node().value;
            that.updatePlot(that.activeYear, xValue, yValue, cValue);
        });

        /* X DROPDOWN */
        let dropX = dropDownWrapper.select('#dropdown_x').select('.dropdown-content').select('select');

        let optionsX = dropX.selectAll('option')
            .data(dropData);

        optionsX.exit().remove();

        let optionsXEnter = optionsX.enter()
            .append('option')
            .attr('value', (d, i) => d.indicator);

        optionsXEnter.append('text')
            .text((d, i) => d.indicator_name);

        optionsX = optionsXEnter.merge(optionsX);

        let selectedX = optionsX.filter(d => d.indicator === xIndicator)
            .attr('selected', true);

        dropX.on('change', function(d, i) {
            let xValue = this.options[this.selectedIndex].value;
            let yValue = dropY.node().value;
            let cValue = dropC.node().value;
            that.updatePlot(that.activeYear, xValue, yValue, cValue);
        });

        /* Y DROPDOWN */
        let dropY = dropDownWrapper.select('#dropdown_y').select('.dropdown-content').select('select');

        let optionsY = dropY.selectAll('option')
            .data(dropData);

        optionsY.exit().remove();

        let optionsYEnter = optionsY.enter()
            .append('option')
            .attr('value', (d, i) => d.indicator);

        optionsY = optionsYEnter.merge(optionsY);

        optionsYEnter.append('text')
            .text((d, i) => d.indicator_name);

        let selectedY = optionsY.filter(d => d.indicator === yIndicator)
            .attr('selected', true);

        dropY.on('change', function(d, i) {
            let yValue = this.options[this.selectedIndex].value;
            let xValue = dropX.node().value;
            let cValue = dropC.node().value;
            that.updatePlot(that.activeYear, xValue, yValue, cValue);
        });

    }

    /**
     * Draws the year bar and hooks up the events of a year change
     */
    drawYearBar() {

        // ******* TODO: PART 2 *******
        //The drop-down boxes are set up for you, but you have to set the slider to updatePlot() on activeYear change

        // Create the x scale for the activeYear;
        // hint: the domain should be max and min of the years (1800 - 2020); it's OK to set it as numbers
        // the plot needs to update on move of the slider

        /* ******* TODO: PART 3 *******
        You will need to call the updateYear() function passed from script.js in your activeYear slider
        */
        let that = this;

        //Slider to change the activeYear of the data
        let yearScale = d3.scaleLinear().domain([1800, 2020]).range([30, 730]);

        let yearSlider = d3.select('#activeYear-bar')
            .append('div').classed('slider-wrap', true)
            .append('input').classed('slider', true)
            .attr('type', 'range')
            .attr('min', 1800)
            .attr('max', 2020)
            .attr('value', this.activeYear);

        let sliderLabel = d3.select('.slider-wrap')
            .append('div').classed('slider-label', true)
            .append('svg');

        let sliderText = sliderLabel.append('text').text(this.activeYear);

        sliderText.attr('x', yearScale(this.activeYear));
        sliderText.attr('y', 25);

        yearSlider.on('input', function() {
            //YOUR CODE HERE  
            let curYear = yearSlider.node().value;
            that.activeYearBG.html(curYear);
            sliderText.text(curYear);
            let xInd = d3.select('#dropdown_x').select('.dropdown-content').select('select').node().value;
            let yInd = d3.select('#dropdown_y').select('.dropdown-content').select('select').node().value;
            let circSizeInd = d3.select('#dropdown_c').select('.dropdown-content').select('select').node().value;
            that.updatePlot(curYear, xInd, yInd, circSizeInd);
            that.updateYear(curYear)
            that.updateCountry()
        });
    }


    /**
     * Draws the legend for the circle sizes
     *
     * @param min minimum value for the sizeData
     * @param max maximum value for the sizeData
     */
    drawLegend(min, max) {
        // ******* TODO: PART 2*******
        //This has been done for you but you need to call it in updatePlot()!
        //Draws the circle legend to show size based on health data
        let scale = d3.scaleSqrt().range([3, 20]).domain([min, max]);

        let circleData = [min, max];

        let svg = d3.select('.circle-legend').select('svg').select('g');

        let circleGroup = svg.selectAll('g').data(circleData);
        circleGroup.exit().remove();

        let circleEnter = circleGroup.enter().append('g');
        circleEnter.append('circle').classed('neutral', true);
        circleEnter.append('text').classed('circle-size-text', true);

        circleGroup = circleEnter.merge(circleGroup);

        circleGroup.attr('transform', (d, i) => 'translate(' + ((i * (5 * scale(d))) + 20) + ', 25)');

        circleGroup.select('circle').attr('r', (d) => scale(d));
        circleGroup.select('circle').attr('cx', '0');
        circleGroup.select('circle').attr('cy', '0');
        let numText = circleGroup.select('text').text(d => new Intl.NumberFormat().format(d));

        numText.attr('transform', (d) => 'translate(' + ((scale(d)) + 10) + ', 0)');
    }

    /**
     * Reacts to a highlight/click event for a country; draws that country darker
     * and fades countries on other continents out
     * @param activeCountry
     */
    updateHighlightClick(activeCountry) {
        /* ******* TODO: PART 3*******
        //You need to assign selected class to the target country and corresponding region
        // Hint: If you followed our suggestion of using classes to style
        // the colors and markers for countries/regions, you can use
        // d3 selection and .classed to set these classes on here.
        // You will not be calling this directly in the gapPlot class,
        // you will need to call it from the updateHighlight function in script.js
        */
        //YOUR CODE HERE  
        this.clearHighlight()
        let plottedCircles = d3.select(".plot-svg").selectAll("circle");
        let curCountry = plottedCircles.filter(d => (d.id.toUpperCase() == activeCountry.toUpperCase()));
        let curRegion = curCountry.datum().region;
        //"hide" the circles that are not in the desired region
        plottedCircles.filter(d => (d.region != curRegion)).classed("hidden", true);
        //highlight the circles that are in the desired region
        plottedCircles.filter(d => (d.region == curRegion)).classed("selected-region", true);
        curCountry.classed("selected-country", true);
    }

    /**
     * Clears any highlights
     */
    clearHighlight() {
        // ******* TODO: PART 3*******
        // Clear the map of any colors/markers; You can do this with inline styling or by
        // defining a class style in styles.css

        // Hint: If you followed our suggestion of using classes to style
        // the colors and markers for hosts/teams/winners, you can use
        // d3 selection and .classed to set these classes off here.

        //YOUR CODE HERE  
        d3.select(".plot-svg").selectAll("circle")
          .classed("hidden", false)
          .classed("selected-country", false)
          .classed("selected-region", false);
    }

    /**
     * Returns html that can be used to render the tooltip.
     * @param data 
     * @returns {string}
     */
    tooltipRender(data) {
        let text = "<h2>" + data['country'] + "</h2>";
        return text;
    }

}