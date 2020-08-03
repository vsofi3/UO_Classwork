/**
 * Makes the first bar chart appear as a staircase.
 *
 * Note: use only the DOM API, not D3!
 */
function staircase() {
  // ****** TODO: PART II ******
  var rects = document.querySelectorAll("#aBar");
  for (var i = 0; i < rects.length; i++) {
    var newWidth = (i * 15) + 10;
    rects[i].setAttribute("width", newWidth);
  }
}

/**
 * Render the visualizations
 * @param data
 */
function update(data) {
  /**
   * D3 loads all CSV data as strings. While Javascript is pretty smart
   * about interpreting strings as numbers when you do things like
   * multiplication, it will still treat them as strings where it makes
   * sense (e.g. adding strings will concatenate them, not add the values
   * together, or comparing strings will do string comparison, not numeric
   * comparison).
   *
   * We need to explicitly convert values to numbers so that comparisons work
   * when we call d3.max()
   **/

  for (let d of data) {
    d.a = +d.a; //unary operator converts string to number
    d.b = +d.b; //unary operator converts string to number
  }

  // Set up the scales
  // TODO: The scales below are examples, modify the ranges and domains to suit your implementation.
  let aScale = d3
    .scaleLinear()
    .domain([0, d3.max(data, d => d.a)])
    .range([0, 140]);
  let bScale = d3
    .scaleLinear()
    .domain([0, d3.max(data, d => d.b)])
    .range([0, 140]);
  let iScale = d3
    .scaleLinear()
    .domain([0, data.length])
    .range([10, 120]);

  // ****** TODO: PART III (you will also edit in PART V) ******

  // TODO: Select and update the 'a' bar chart bars
  var aData = [];
  var bData = [];

  for (var i = 0; i < data.length; i++){
    aData.push(data[i]["a"]);
    bData.push(data[i]["b"]);
  }
  var aBar = d3.selectAll("#aBar")
      .attr("width", (d,i) => aScale(aData[i]))
      .data(aData)
      .on("mouseover", function(d) {
            d3.select(this).attr("r", 10).style("fill", "#ff8080");
          })                  
      .on("mouseout", function(d) {
            d3.select(this).attr("r", 5.5).style("fill", "#c7001e");
          });;

  // TODO: Select and update the 'b' bar chart bars
  var bBar = d3.selectAll("#bBar")
      .attr("width", (d,i) => bScale(bData[i]))
      .data(bData)
      .on("mouseover", function(d) {
            d3.select(this).attr("r", 10).style("fill", "#b3d9ff");
          })                  
      .on("mouseout", function(d) {
            d3.select(this).attr("r", 5.5).style("fill", "#086fad");
          });;

  // TODO: Select and update the 'a' line chart path using this line generator
  let aLineGenerator = d3
    .line()
    .x((d, i) => i)
    .y(d => d.a);

   var aLine = d3.select("#aLine").selectAll("path")
      .transition()
      .duration(500)
      .attr("opacity", 1)
      .attr("d", aLineGenerator(data))

  // TODO: Select and update the 'b' line chart path (create your own generator)
  
  let bLineGenerator = d3
    .line()
    .x((d, i) => i)
    .y(d => d.b);

  var bLine = d3.select("#bLine").selectAll("path")
    .transition()
    .duration(500)
    .attr("opacity", 1)
    .attr("d", bLineGenerator(data))

  // TODO: Select and update the 'a' area chart path using this area generator
  
  let aAreaGenerator = d3
    .area()
    .x((d, i) => i)
    .y0(0)
    .y1(d => d.a);

  var aArea = d3.select("#aArea").selectAll("path")
    .transition()
    .duration(500)
    .attr("opacity", 1)
    .attr("d", aAreaGenerator(data))

  // TODO: Select and update the 'b' area chart path (create your own generator)

  let bAreaGenerator = d3
      .area()
      .x((d,i) => i)
      .y0(0)
      .y1(d => d.b);

  var bArea = d3.select("#bArea").selectAll("path")
    .transition()
    .duration(500)
    .attr("opacity", 1)
    .attr("d", bAreaGenerator(data))

  // TODO: Select and update the scatterplot points
  // var xAxis = d3.axisBottom()
  //   .scale(aScale(data.a))
  // var yAxis = d3.axisLeft()
  //   .scale(bScale(data.b))

  var scatter = d3.select("#scatter").selectAll("circle")
    .attr("cx", (d,i) => aScale(aData[i]))
    .attr("cy", (d,i) => bScale(bData[i]))
    .attr("r", 3)
    .on("click", function(d, i) {
            console.log("X: " + aScale(aData[i]) + "    Y: " + bScale(bData[i]))
          })  

  // var AxisX = d3.select("#scatter").select("#x-axis")
  //   .call(xAxis)

  // var AxisY = d3.select("#scatter").select("#y-axis")
  //   .call(yAxis)


}

/**
 * Update the data according to document settings
 */
async function changeData() {
  //  Load the file indicated by the select menu
  let dataFile = document.getElementById("dataset").value;
  try {
    const data = await d3.csv("data/" + dataFile + ".csv");
    if (document.getElementById("random").checked) {
      // if random
      update(randomSubset(data)); // update w/ random subset of data
    } else {
      // else
      update(data); // update w/ full data
    }
  } catch (error) {
    alert("Could not load the dataset!");
  }
}

/**
 *  Slice out a random chunk of the provided in data
 *  @param data
 */
function randomSubset(data) {
  return data.filter(d => Math.random() > 0.5);
}
