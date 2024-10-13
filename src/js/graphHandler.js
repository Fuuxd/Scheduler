// Wait for the SVG <object> to be fully loaded
document.getElementById('graph').addEventListener('load', function() {
    // Access the inner document of the loaded SVG
    var svgDoc = this.contentDocument; // Access the SVG document inside the <object>
    let checkedNodes = new Set(); // To keep track of checked nodes

    // Select all nodes (g.node elements) from the loaded SVG document
    svgDoc.querySelectorAll('g.node').forEach(function(node) {
        // Add click event listener to each node
        node.addEventListener('click', function() {
            let polygon = node.querySelector('polygon'); // Get the polygon inside the node group
            if (polygon) {
                if (checkedNodes.has(node.id)) {
                    // If already checked, uncheck by reverting color and removing from the set
                    polygon.setAttribute('fill', 'white'); // Revert to original color (empty string)
                    checkedNodes.delete(node.id); // Remove from the set
                } else {
                    // Otherwise, check by changing color and adding to the set
                    polygon.setAttribute('fill', 'red'); // Change to red on click
                    checkedNodes.add(node.id); // Add to the set
                }
            }
        });
    });

    // Add functionality to submit button to list checked nodes
    document.getElementById('submitBtn').addEventListener('click', function() {
        let checkedList = Array.from(checkedNodes); // Convert the set to an array

        const nodeCount = checkedList.length;

        if (nodeCount > 0) {
            fetch('/calculate', {
                method: 'POST',
                headers: {
                    'Content-Type': 'application/json'
                },
                body: JSON.stringify({ nodes: checkedList }) // Send the title list to the server
            })
            .then(response => response.text())
            .then(result => {
                // Display the result returned by the C++ program
                document.getElementById('result').innerText = result; // Ensure you use 'result' not 'data'
            })
            .catch(error => console.error('Error:', error));
        } else {
            console.log('Nothing picked. Nothing sent.');
        }
    });
});