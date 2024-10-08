const express = require('express');
const { exec } = require('child_process'); // To run C++ executable
//const cors = require('cors'); // To allow cross-origin requests from your webpage
const bodyParser = require('body-parser');

const app = express();
const port = 3000;

//app.use(cors());
app.use(express.json()); // To parse JSON body in requests

// Serve static files like HTML, JS, CSS from the root directory (or you can specify a different directory)
app.use(express.static(__dirname));

// Endpoint to handle node list submission
app.post('/calculate', (req, res) => {
    const checkedNodes = req.body.nodes; // Get the list of checked nodes from the request

    const nodeCount = checkedNodes.length;

    if (nodeCount > 0) {
        const nodeList = checkedNodes.join(' '); // Create a space-separated string of node IDs
        const command = `./scheduleCreator ${nodeCount} ${nodeList}`; // Assuming your C++ program takes node IDs as arguments

        // Execute the C++ program
        exec(command, (error, stdout, stderr) => {
            if (error) {
                console.error(`Error executing C++ program: ${stderr}`);
                res.status(500).send('Error executing C++ program');
                return;
            }

            // Send the multi-line string result back to the client
            res.send(stdout);
        });
    } else {
        res.status(400).send('No nodes were selected');
    }
});

// Start the server
app.listen(port, () => {
    console.log(`Server is running on http://localhost:${port}`);
});
