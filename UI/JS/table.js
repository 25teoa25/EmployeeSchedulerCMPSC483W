document.addEventListener("DOMContentLoaded", function() {
    const tableBody = document.getElementById("shift-table-body");

    if (tableBody) {
        console.log(tableBody);  // Check if the table body is found
        
        const shifts1 = [
            { day: "Monday", pediatric: true, oncology: true },
            { day: "Tuesday", pediatric: true, oncology: true },
            { day: "Wednesday", pediatric: true, oncology: true },
            { day: "Thursday", pediatric: true, oncology: true },
            { day: "Friday", pediatric: true, oncology: true },
            { day: "Saturday", pediatric: true, oncology: true },
            { day: "Sunday", pediatric: true, oncology: true },
            { day: "Monday", pediatric: true, oncology: true },
            { day: "Tuesday", pediatric: true, oncology: true },
            { day: "Wednesday", pediatric: true, oncology: true },
            { day: "Thursday", pediatric: true, oncology: true },
            { day: "Friday", pediatric: true, oncology: true },
            { day: "Saturday", pediatric: true, oncology: true },
            { day: "Sunday", pediatric: true, oncology: true }
        ];

        let row = document.createElement('tr');
        
        // Add the first column (header for the shift)
        row.innerHTML = `<th class="shift-row">Shift 1 <br> 7am-3pm</th>`;
        
        // Loop through each day and add data for each day to the same row
        shifts1.forEach(shift => {
            let cell = document.createElement('td');
            if (shift.pediatric && shift.oncology){
                cell.innerHTML = `
                    <p class="pediatric">Pediatric</p>
                    <p class="oncology">Oncology</p>
                `;
            }
            row.appendChild(cell); // Append cell to the row
        });

        // Append the entire row to the table body
        tableBody.appendChild(row);

    } else {
        console.error("Element with ID #shift-table-body not found.");
    }
});
