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
        //Fill tab header with all departments
        document.getElementById("tabs").innerHTML += `<button onClick="switchTab('${department}')">${department}</button>`;
    }
    //Create a table for each department (hide by default)
    for (let department of arrDepartments) {
        //console.log('Creating table: ' + department);
        let departmentID = department.toLowerCase();
        departmentID = departmentID.concat("Schedule");
        console.log(departmentID);
        let container = document.getElementById("table-container");
        //console.log(container);
        document.getElementById("table-container").innerHTML += 
        `<div class="schedule" id="${departmentID}">
            <table>
                <thead>
                    <tr class="shift-row">
                        <th class="shift-col"></th>
                        <th class="shift-col">${department}Monday (9/30)</th>
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
                <tbody id="shift-table-body">
                </tbody>
            </table>
        </div>`;
    }
}
function switchTab (tabSelected) {
    //Change header at top of page
    currentTab = tabSelected;
    console.log(currentTab);
    document.getElementById('tabHeader').innerHTML = `${currentTab} Schedule`;
    //Switch table
    for (let department of arrDepartments) {
        let oldTabID = department.toLowerCase();
        oldTabID = oldTabID.concat("Schedule");
        //console.log(oldTabID);
        let oldTab = document.getElementById(oldTabID);
        oldTab.setAttribute("hidden", "hidden");
        console.log('Hiding tab: ' + oldTabID);
    }
    let selectedTabID = tabSelected.toLowerCase();
    selectedTabID = selectedTabID.concat("Schedule"); 
    console.log('Showing tab ' + selectedTabID);
    let selectedTab = document.getElementById(selectedTabID);
    selectedTab.removeAttribute("hidden");
}