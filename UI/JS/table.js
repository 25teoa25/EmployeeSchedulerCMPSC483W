document.addEventListener("DOMContentLoaded", function() {
    const tableBody = document.getElementById("shift-table-body");

    if (tableBody) {
        console.log(tableBody);
        
        // Fetch the JSON file and parse it
        fetch('test.json')
            .then(response => {
                if (!response.ok) {
                    throw new Error('Failed to fetch the JSON file');
                }
                return response.json();
            })
            .then(data => {
                console.log(data.shifts1);
                console.log(data.shifts2);

                // Process shifts1 and create the first row (Shift 1)
                let row1 = document.createElement('tr');
                row1.innerHTML = `<th class="shift-row">Shift 1 <br> 7am-3pm</th>`;
                
                data.shifts1.forEach(shift => {
                    let cell = document.createElement('td');
                    if (shift.pediatric) {
                        cell.innerHTML += `<p class="pediatric">Pediatric</p>`;
                    }
                    if (shift.oncology) {
                        cell.innerHTML += `<p class="oncology">Oncology</p>`;
                    }
                    row1.appendChild(cell);
                });

                tableBody.appendChild(row1);

                // Process shifts2 and create the second row (Shift 2)
                let row2 = document.createElement('tr');
                row2.innerHTML = `<th class="shift-row">Shift 2 <br> 3pm-11pm</th>`;
                
                data.shifts2.forEach(shift => {
                    let cell = document.createElement('td');
                    if (shift.pediatric) {
                        cell.innerHTML += `<p class="pediatric">Pediatric</p>`;
                    }
                    if (shift.oncology) {
                        cell.innerHTML += `<p class="oncology">Oncology</p>`;
                    }
                    row2.appendChild(cell);
                });

                tableBody.appendChild(row2);
            })
            .catch(error => {
                console.error('Error loading JSON:', error);
            });

    } else {
        console.error("Element with ID #shift-table-body not found.");
    }
});
