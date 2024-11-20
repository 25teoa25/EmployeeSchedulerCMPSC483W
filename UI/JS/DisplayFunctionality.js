let currentTab = "Overall";
const basicDistinctColors = [
    "Red",
    "Orange",
    "Yellow",
    "Green",
    "Blue",
    "Purple",
    "Cyan",
    "Magenta",
    "Brown",
    "Pink",
    "Lime",
    "Teal",
    "Navy",
    "Gold",
];
let arrDepartments = [];
/*
    Need some kind of function that reads in the JSON file (or whatever other input) and makes list of all departments
    Can store list of departments as array then change tabs and tabSelected's to elements of that array
*/
async function fillDepartments() {
    try {
        const response = await fetch('http://localhost:8000/DataStructure/LinkedListDS/shift_schedule.json');
        if (!response.ok) {
            throw new Error('Failed to fetch the JSON file');
        }
        const data = await response.json();

        let departmentsfill = ["Overall"];
        let typesfill = [];
        for (let shift of data) {
            for (let nurse of shift.nurses) {
                if (!departmentsfill.includes(nurse.department)) {
                    departmentsfill.push(nurse.department);
                }
                if (!typesfill.includes(nurse.nurseType)) {
                    typesfill.push(nurse.nurseType);
                }
            }
        }
        return { departmentsfill, typesfill };
    } catch (error) {
        console.error('Error fetching departments:', error);
        return [];
    }
}

async function fillTabs () {
    const { departmentsfill, typesfill } = await fillDepartments();
    arrDepartments = departmentsfill;
    let departmentColors = {};
    let typeColors = {};
    arrDepartments.forEach((department, index) => {
        const color = basicDistinctColors[index % basicDistinctColors.length]; // Cycle through the color array
        departmentColors[department] = color;
    });
    typesfill.forEach((type, index) => {
        const color = basicDistinctColors[index % basicDistinctColors.length]; // Cycle through the color array
        typeColors[type] = color;
    });
    console.log("Array Departments:", arrDepartments);
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
                        <th class="shift-col">Monday (9/30)</th>
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
        loadDepartmentData(department, departmentID, departmentColors, typeColors);
    }
}

function loadDepartmentData(department, departmentID, departmentColors, typeColors) {
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
                if (department == "Overall") {
                    let row1 = null;
                    for (let shift of data) {
                        let departmentPerShift = [];
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
                            if (!departmentPerShift.includes(nurse.department)) {
                                departmentPerShift.push(nurse.department);
                            }
                        }
                        departmentPerShift.sort((a, b) => a.localeCompare(b));
                        for (let dept of departmentPerShift) {
                            let color = departmentColors[dept];
                            cell.innerHTML += `<p style="background-color: ${color}">${dept}</p>`;
                        }
                        if (row1) {
                            row1.appendChild(cell);
                            tableBody.appendChild(row1);
                        }
                    }
                }
                else {
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
                            if (nurse.department === department) {
                                let color = typeColors[nurse.nurseType];
                                cell.innerHTML += `<p style="background-color: ${color}">${nurse.name}</p>`;
                            }
                        }

                        if (row1) {
                            row1.appendChild(cell);
                            tableBody.appendChild(row1);
                        }
                    }
                }
            })
            .catch(error => {
                console.error('Error loading JSON:', error);
            });
    }, 100);
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

async function startupFunction() {
    await fillTabs(); // Wait until tabs are created
    switchTab("Overall"); // Switch to the default tab
}

function myFunction() {
    var popup = document.getElementById("myPopup");
    popup.classList.toggle("show");
  }