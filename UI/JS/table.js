// document.addEventListener("DOMContentLoaded", function() {
//     const tableBody = document.getElementById("shift-table-body");

//     if (tableBody) {
//         console.log(tableBody);  // Check if the table body is found
        
//         fetch('UI/test.json')
//             .then(response => {
//                 if (!response.ok) {
//                     throw new Error('Failed to fetch the JSON file');
//                 }
//                 return response.json();
//             })
//             .then(data => {
//                 console.log(data.shifts1);

//                 // Now that the data is fetched, process it here
//                 let row = document.createElement('tr');
                        
//                 // Add the first column (header for the shift)
//                 row.innerHTML = `<th class="shift-row">Shift 1 <br> 7am-3pm</th>`;
                
//                 // Loop through each shift in shifts1 and add data for each day to the same row
//                 data.shifts1.forEach(shift => {
//                     let cell = document.createElement('td');
//                     if (shift.pediatric) {
//                         cell.innerHTML += `
//                             <p class="pediatric">Pediatric</p>
//                         `;
//                     }
//                     if (shift.oncology) {
//                         cell.innerHTML += `
//                             <p class="oncology">Oncology</p>
//                         `;
//                     }
//                     row.appendChild(cell); // Append cell to the row
//                 });

//                 // Append the entire row to the table body
//                 tableBody.appendChild(row);
//             })


//         // let row2 = document.createElement('tr');
        
//         // // Add the first column (header for the shift)
//         // row2.innerHTML = `<th class="shift-row">Shift 2 <br> 3pm-11pm</th>`;
        
//         // // Loop through each day and add data for each day to the same row
//         // shifts2.forEach(shift => {
//         //     let cell = document.createElement('td');
//         //     if (shift.pediatric){
//         //         cell.innerHTML += `
//         //             <p class="pediatric">Pediatric</p>
//         //         `;
//         //     }
//         //     if (shift.oncology) {
//         //         cell.innerHTML += `
//         //             <p class="oncology">Oncology</p>
//         //         `;
//         //     }
//         //     row2.appendChild(cell); // Append cell to the row
//         // });

//         // // Append the entire row to the table body
//         // tableBody.appendChild(row2);

//     } else {
//         console.error("Element with ID #shift-table-body not found.");
//     }
// });

document.addEventListener("DOMContentLoaded", function() {
    const tableBody = document.getElementById("shift-table-body");

    if (tableBody) {
        console.log(tableBody);  // Check if the table body is found
        
        // Fetch the JSON file and parse it
        fetch('test.json')  // Ensure this file path is correct relative to your HTML file
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
