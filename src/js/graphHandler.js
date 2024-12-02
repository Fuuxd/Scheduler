function loadCheckboxList() {
    fetch('checkboxList.html')
        .then(response => response.text())
        .then(html => {
            document.getElementById('checkboxContainer').innerHTML = html;

            // Add event listeners to dynamically loaded content
            document.querySelectorAll('.checkbox').forEach(item => {
                item.addEventListener('click', function () {
                    const value = parseInt(this.getAttribute('data-value'), 10);

                    // Toggle the checked class and track selections
                    if (this.classList.contains('checked')) {
                        this.classList.remove('checked');
                        checkedNodes.delete(value);
                    } else {
                        this.classList.add('checked');
                        checkedNodes.add(value);
                    }
                });
            });
        })
        .catch(error => console.error('Error loading checkbox list:', error));
}


const checkedNodes = new Set();

// Add click event listeners to each checkbox text
document.querySelectorAll('.checkbox').forEach(item => {
    item.addEventListener('click', function () {
        const value = parseInt(this.getAttribute('data-value'), 10);

        // Toggle the checked class and track selections
        if (this.classList.contains('checked')) {
            this.classList.remove('checked');
            checkedNodes.delete(value);
        } else {
            this.classList.add('checked');
            checkedNodes.add(value);
        }
    });
});

// Handle form submission
document.getElementById('submitBtn').addEventListener('click', function () {
    let checkedList = Array.from(checkedNodes); // Convert the set to an array

    const nodeCount = checkedList.length;
    console.log('Selected Values:', checkedNodes);

    if (nodeCount > 0) {
        fetch('/calculate', {
            method: 'POST',
            headers: {
                'Content-Type': 'application/json'
            },
            body: JSON.stringify({ nodes: checkedList }) // Send the values list to the server
        })
        .then(response => response.text())
        .then(result => {
            // Display the result returned by the server
            document.getElementById('result').innerText = result;
        })
        .catch(error => console.error('Error:', error));
    } else {
        console.log('Nothing picked. Nothing sent.');
    }
});