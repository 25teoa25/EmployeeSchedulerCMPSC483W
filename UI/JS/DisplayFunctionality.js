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
        let departmentID = department.toLowerCase().replace(/\s+/g, "");
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

function searchNurse() {
    //Take value from search field
    var nurseSearched = document.getElementById("search-field").value;
    console.log('Searching for nurse: ' + nurseSearched);
    //Go back through JSON file
    fetch('http://localhost:8000/DataStructure/LinkedListDS/shift_schedule.json')
            .then(response => {
                if (!response.ok) {
                    throw new Error('Failed to fetch the JSON file');
                }
                return response.json();
            })
            .then(data => {
                var nurseFound = false;
                var nurseName;
                var nurseType;
                //Loop through each nurse of each shift (all elements in JSON)
                var shiftsWorked = []
                for (let shift of data) {
                    for (let nurse of shift.nurses) {
                        //If match found return nurse info
                        if (nurse.name.toLowerCase() == nurseSearched.toLowerCase()) {
                            console.log('Name: ' + nurse.name + " Type: " + nurse.nurseType);
                            nurseName = nurse.name;
                            nurseType = nurse.nurseType;
                            nurseFound = true;
                            //Switch department tab to department nurse is in
                            var department = nurse.department;
                            switchTab(department);
                            shiftsWorked.push(shift["1shift"]);
                            console.log(shiftsWorked);
                        }

                    }
                }
                if (nurseFound == false) {
                    console.log("Not found");
                    document.getElementById("above-search").innerHTML = `<p class="search-error">NURSE NOT FOUND</p>`;
                }
                else {
                    document.getElementById("above-search").innerHTML =
                    `<p style = "font-weight: bold; text-decoration: underline;">${nurseName}: ${nurseType}</p>
                    <div class = "search-table-container" id = "search-table-container" style = "display: flex; justify-content: flex-start; margin: 20px 0; width: 45%; min-width: 200px;">
                        <table id = "search-table-1">
                            <tbody>
                                <tr>
                                    <th>Week 1</th>
                                </tr>
                            </tbody>
                        </table>
                        <table id = "search-table-2">
                            <tbody>
                                <tr>
                                    <th>Week 2</th>
                                </tr>
                            </tbody>
                        </table>
                    </div>`;
                    //Loop through all shifts and place in correct row of correct table
                    for (let shift of shiftsWorked) {
                        var shiftNum;
                        //Shift 1 of a day
                        if (shift % 3 == 1) {
                            shiftNum = '1';
                        }
                        //Shift 2 of a day
                        if (shift % 3 == 2) {
                            shiftNum = '2';
                        }
                        //Shift 3 of a day
                        if (shift % 3 == 0) {
                            shiftNum = '3';
                        }
                        //Determine the day of the week the shift is part of 
                        var shiftDay;
                        if ((shift <= 3 && shift >= 1) || (shift <= 24 && shift >= 22)) {
                            shiftDay = 'Monday';
                        }
                        else if ((shift <= 6 && shift >= 4) || (shift <= 27 && shift >= 25)) {
                            shiftDay = 'Tuesday';
                        }
                        else if ((shift <= 9 && shift >= 7) || (shift <= 30 && shift >= 28)) {
                            shiftDay = 'Wednesday';
                        }
                        else if ((shift <= 12 && shift >= 10) || (shift <= 33 && shift >= 31)) {
                            shiftDay = 'Thursday';
                        }
                        else if ((shift <= 15 && shift >= 13) || (shift <= 35 && shift >= 33)) {
                            shiftDay = 'Friday';
                        }
                        else if ((shift <= 18 && shift >= 16) || (shift <= 38 && shift >= 36)) {
                            shiftDay = 'Saturday';
                        }
                        else if ((shift <= 21 && shift >= 19) || (shift <= 42 && shift >= 39)) {
                            shiftDay = 'Sunday';
                        }
                        else {
                            shiftDay = shift;
                        }
                        //Determine which week (and table) the shift should go in
                        var tableID;
                        if (shift <= 21) {
                            tableID = "search-table-1";
                        }
                        else {
                            tableID = "search-table-2";
                        }
                        document.getElementById(tableID).innerHTML += 
                        `<tr>
                            <td style = "font-size: 12px;">${shiftDay} Shift ${shiftNum}</td>
                        </tr>`;
                    }
                }
            })
            .catch(error => {
                console.error('Error loading JSON:', error);
            });
}

function switchTab (tabSelected) {
    //Change header at top of page
    currentTab = tabSelected;
    document.getElementById('tabHeader').innerHTML = `${currentTab} Schedule`;
    //Switch table
    for (let department of arrDepartments) {
        let oldTabID = department.toLowerCase().replace(/\s+/g, "");
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