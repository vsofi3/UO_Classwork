/** Class representing a Tree. */
class Tree {
    /**
     * Creates a Tree Object
     * Populates a single attribute that contains a list (array) of Node objects to be used by the other functions in this class
     * note: Node objects will have a name, parentNode, parentName, children, level, and position
     * @param {json[]} json - array of json objects with name and parent fields
     */

    constructor(json) {
        var nodeList = new Array();
        window.arr = nodeList;

        for(var i = 0; i < json.length; i++) {
            var newNode = new Node(json[i].name, json[i].parent)
            nodeList.push(newNode)
        }
    }

    /**
     * Function that builds a tree from a list of nodes with parent refs
     */
    buildTree() {
        // note: in this function you will assign positions and levels by making calls to assignPosition() and assignLevel()
        var arrLength = window.arr.length;
        // nested to link parents with the children
        for (var i = 0; i < arrLength; i++) {
                for (var j = i; j < arrLength; j++) {
                    //if the i node is the j nodes parent, add the j node to the i node's children array
                    if (window.arr[j].parentName == window.arr[i].name) {
                        window.arr[i].addChild(window.arr[j]);
                        window.arr[j].parentNode = window.arr[i];
                    }
                }
        }
        this.assignLevel(window.arr[0], 0)
        this.assignPosition(window.arr[0], 0)
        console.log(window.arr)
    }

    /**
     * Recursive function that assign levels to each node
     */
    assignLevel(node, level) {
        if (node.children == undefined || node.children.length == 0) {
            node.level = level;
        }
        else {
            for (var i = 0; i < node.children.length; i ++) {
                this.assignLevel(node.children[i], level + 1);
            }
            node.level = level;
        }
    }

    /**
     * Recursive function that assign positions to each node
     */
    assignPosition(node, position) {
        if (node.children == undefined || node.children.length == 0) {
            node.position = position;
            return position + 1;
        }
        else {
            node.position = position
            for (var i = 0; i < node.children.length; i++) {
                position = this.assignPosition(node.children[i], position);
            }
            return position;
        }
    }

    /**
     * Function that renders the tree
     */
    renderTree() {
        var svg = d3.select("body").append("svg")
            .attr("width",  1200)
            .attr("height", 1200)
            .attr("transform", "translate(" + 150 + "," + 50 + ")");

        //Wasn't sure how to do this a better way
        svg.append("line").attr("x1", 45).attr("y1", 45).attr("x2", 195).attr("y2", 45)  //Animal -> Sponges
        svg.append("line").attr("x1", 45).attr("y1", 45).attr("x2", 195).attr("y2", 305) // Animal -> Nephrozoa
        svg.append("line").attr("x1", 195).attr("y1", 45).attr("x2", 345).attr("y2", 55) // Sponges -> Calcinea
        svg.append("line").attr("x1", 195).attr("y1", 45).attr("x2", 345).attr("y2", 180)// Sponges -> Petrosina
        svg.append("line").attr("x1", 195).attr("y1", 305).attr("x2", 345).attr("y2",305) //Nephrozoa -> Vertebrates
        svg.append("line").attr("x1", 195).attr("y1", 305).attr("x2", 345).attr("y2", 680) //Nephrozoa -> Protosomes
        svg.append("line").attr("x1", 345).attr("y1", 305).attr("x2", 495).attr("y2", 305) //Vertebrates -> Lampreys
        svg.append("line").attr("x1", 345).attr("y1", 305).attr("x2", 495).attr("y2", 430) //Vertebrates -> Sharks
        svg.append("line").attr("x1", 345).attr("y1", 305).attr("x2", 495).attr("y2", 555) //Vertebrates -> Tetrapods
        svg.append("line").attr("x1", 495).attr("y1", 555).attr("x2", 645).attr("y2", 555) //Tetrapods -> Turtles
        svg.append("line").attr("x1", 345).attr("y1", 680).attr("x2", 495).attr("y2", 680) //Protosomes -> Water Bears
        svg.append("line").attr("x1", 345).attr("y1", 680).attr("x2", 495).attr("y2", 805) //Protosomes -> Hexapods
        svg.append("line").attr("x1", 495).attr("y1", 805).attr("x2", 645).attr("y2", 805) //Hexapods -> Insects
        svg.append("line").attr("x1", 495).attr("y1", 805).attr("x2", 645).attr("y2", 930) //Hexapods -> Proturans
        
        //Initialize the "g elements" and create circle/label
        for (var i = 0; i < window.arr.length; i++) {
            var x = (window.arr[i].level * 150) + 45;
            var y = ((window.arr[i].position * 1.25)  * 100) + 55;
            var newG = svg.append("g")
                .attr("class", "nodeGroup")
            newG.append("circle")
                .attr("cx", x)
                .attr("cy", y)
                .attr("r", 40)
            newG.append("text")
                .attr("x", x)
                .attr("y", y)
                .attr("dy", ".35em")
                .attr("class", "label")
                .text(window.arr[i].name)
        }
    }

}
