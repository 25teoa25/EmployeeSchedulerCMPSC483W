let currentTab = "Overall";
/*
    Need some kind of function that reads in the JSON file (or whatever other input) and makes list of all departments
    Can store list of departments as array then change tabs and tabSelected's to elements of that array
*/
let arrDepartments = ["Overall", "Oncology", "Pediatric", "Surgery"];
function fillTabs () {
    console.log(arrDepartments);
    for (let department of arrDepartments) {
        console.log('Inserting tab:' + department);
        // Fill tab header with all departments
        document.getElementById("tabs").innerHTML += `<button onClick="switchTab('${department}')">${department}</button>`;
    }
    // Create a table for each department (hide by default)
    for (let department of arrDepartments) {
        console.log('Creating table: ' + department);
        let departmentID = department.toLowerCase();
        departmentID = departmentID.concat("Schedule");
        let container = document.getElementById("table-container");
        console.log(container);
        console.log(`${departmentID}-shift-table-body`);
        document.getElementById("table-container").innerHTML += 
        `<div class="schedule" id="${departmentID}" hidden>
            <table>
                <thead>
                    <tr class="shift-row">
                        <th class="shift-col"></th>
                        <th class="shift-col">${department} Monday (9/30)</th>
                        <th class="shift-col">Tuesday</th>
                        <th class="shift-col">Wednesday</th>
                        <th class="shift-col">Thursday</th>
                        <th class="shift-col">Friday</th>
                        <th class="shift-col">Saturday</th>
                        <th class="shift-col">Sunday</th>
                        <th class="shift-col">Monday (10/7)</th>
                        <th class="shift-col">Tuesday</th>
                        <th class="shift-col">Wednesday</th>
                        <th class="shift-col">Thursday</th>
                        <th class="shift-col">Friday</th>
                        <th class="shift-col">Saturday</th>
                        <th class="shift-col">Sunday</th>
                    </tr>
                </thead>
                <tbody id="${departmentID}-shift-table-body">
                </tbody>
            </table>
        </div>`;
        loadDepartmentData(department, departmentID);
        loadNurseCount(department);
    }
}

function loadDepartmentData(department, departmentID) {
    setTimeout(() => {
        const tableBody = document.getElementById(`${departmentID}-shift-table-body`);
        if (!tableBody) {
            console.error(`Table body for ${departmentID} is null.`);
            return;
        }

        fetch('http://localhost:8000/DataStructure/LinkedListDS/shift_schedule.json')
            .then(response => {
                if (!response.ok) {
                    throw new Error('Failed to fetch the JSON file');
                }
                return response.json();
            })
            .then(data => {
                let row1 = null;
                for (let shift of data) {

                    // Create a row for the specific shifts
                    if (shift["1shift"] == 1) {
                        row1 = document.createElement('tr');
                        row1.innerHTML = `<th class="shift-row">Shift #1 <br> 7am-3pm</th>`;
                    } else if (shift["1shift"] == 15) {
                        row1 = document.createElement('tr');
                        row1.innerHTML = `<th class="shift-row">Shift #2 <br> 3pm-11pm</th>`;
                    } else if (shift["1shift"] == 29) {
                        row1 = document.createElement('tr');
                        row1.innerHTML = `<th class="shift-row">Shift #3 <br> 11pm-7am</th>`;
                    }

                    let cell = document.createElement('td');
                    for (let nurse of shift.nurses) {

                        console.log(`Nurse: ${nurse.name}`);
                        if (nurse.department === department) {
                            cell.innerHTML += `<p>${nurse.name} (${nurse.nurseType}) <br> (${nurse.department}) (${shift["1shift"]})</p>`;
                        }
                    }

                    if (row1) {
                        row1.appendChild(cell);
                        tableBody.appendChild(row1);
                    }
                }
            })
            .catch(error => {
                console.error('Error loading JSON:', error);
            });
    }, 100); // Wait for DOM updates
}

function switchTab (tabSelected) {
    //Change header at top of page
    currentTab = tabSelected;
    document.getElementById('tabHeader').innerHTML = `${currentTab} Schedule`;
    //Switch table
    for (let department of arrDepartments) {
        let oldTabID = department.toLowerCase();
        oldTabID = oldTabID.concat("Schedule");
        console.log(oldTabID);
        let oldTab = document.getElementById(oldTabID);
        console.log(oldTab);
        oldTab.setAttribute("hidden", "hidden");
        console.log('Hiding tab: ' + oldTabID);
    }
    let selectedTabID = tabSelected.toLowerCase();
    selectedTabID = selectedTabID.concat("Schedule"); 
    console.log('Switching to ' + selectedTabID);
    let selectedTab = document.getElementById(selectedTabID);
    selectedTab.removeAttribute("hidden");
    /*if (tabSelected == 'Pediatric') {
        console.log('Switching to pediatric')
        let selectedTab = document.getElementById('pediatricSchedule');
        let oldTab = document.getElementById('oncologySchedule');
        selectedTab.removeAttribute("hidden");
        oldTab.setAttribute("hidden", "hidden");
    }
    if (tabSelected == 'Oncology') {
        console.log('Switching to oncology')
        let selectedTab = document.getElementById('oncologySchedule');
        let oldTab = document.getElementById('pediatricSchedule');
        selectedTab.removeAttribute("hidden");
        oldTab.setAttribute("hidden", "hidden");
    }
    else {
        console.log('Switching to overall')
    }*/
}
function countAllDepartmentsNurses(outputElementID) {
        fetch('http://localhost:8000/DataStructure/LinkedListDS/shift_schedule.json')
            .then(response => {
                if (!response.ok) {
                    throw new Error('Failed to fetch the JSON file');
                }
                return response.json();
            })
            .then(data => {
                const departmentCounts = {};
                const countedNurses = new Set();
                let totalNurses = 0;
    
                for (let shift of data) {
                    for (let nurse of shift.nurses) {
                        if (!countedNurses.has(nurse.name)) {
                            countedNurses.add(nurse.name);
                            totalNurses++;
    
                            if (nurse.department in departmentCounts) {
                                departmentCounts[nurse.department] += 1;
                            } else {
                                departmentCounts[nurse.department] = 1;
                            }
                        }
                    }
                }
    
                const outputElement = document.getElementById(outputElementID);
                if (outputElement) {
                    outputElement.innerHTML = "";
                    for (let [department, count] of Object.entries(departmentCounts)) {
                        const line = document.createElement('p');
                        outputElement.appendChild(line);
                        line.innerText = `Total nurses in department "${department}": ${count}`;
                        outputElement.appendChild(line);
                    }
    
                    const totalLine = document.createElement('p');
                    totalLine.innerText = `Total nurses counted: ${totalNurses}`;
                    outputElement.appendChild(totalLine);
                }
            })
            
    }
    
    function findUnassignedNurses(outputElementID) {
        fetch('http://localhost:8000/DataStructure/LinkedListDS/shift_schedule.json')
            .then(response => {
                if (!response.ok) {
                    throw new Error('Failed to fetch the JSON file');
                }
                return response.json();
            })
            .then(data => {
                const results = [];
    
                for (let shift of data) {
                    for (let nurse of shift.nurses) {
                        if (nurse.nurseID === -1) {
                            results.push({
                                department: nurse.department,
                                shift: shift["1shift"]
                            });
                        }
                    }
                }
    
                const outputElement = document.getElementById(outputElementID);
                if (outputElement) {
                    outputElement.innerHTML = "";
                    if (results.length > 0) {
                        results.forEach(item => {
                            const line = document.createElement('p');
                            line.innerText = `Department: ${item.department}, Shift: ${item.shift}`;
                            outputElement.appendChild(line);
                        });
                    } else {
                        outputElement.innerText = "No unassigned nurses found.";
                    }
                }
            })
            
    }
    
    function calculateAverageSatisfaction(outputElementID) {
        fetch('http://localhost:8000/DataStructure/LinkedListDS/shift_schedule.json')
            .then(response => {
                if (!response.ok) {
                    throw new Error('Failed to fetch the JSON file');
                }
                return response.json();
            })
            .then(data => {
                let totalSatisfaction = 0;
                let totalNurses = 0;
    
                for (let shift of data) {
                    if (Array.isArray(shift.nurses)) {
                        for (let nurse of shift.nurses) {
                            if (nurse.score !== undefined && !isNaN(nurse.score)) {
                                totalSatisfaction += nurse.score;
                                totalNurses++;
                            }
                        }
                    }
                }
    
                const averageSatisfaction = totalNurses > 0 ? totalSatisfaction / totalNurses : 0;
    
                const outputElement = document.getElementById(outputElementID);
                if (outputElement) {
                    outputElement.innerText = `Average Satisfaction Score: ${averageSatisfaction.toFixed(2)}`;
                }
            })
        
    }
    


