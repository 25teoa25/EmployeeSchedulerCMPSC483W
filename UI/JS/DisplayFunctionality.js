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
    "Navy",
    "Lime",
    "Teal",
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
        const color = basicDistinctColors[basicDistinctColors.length - 1 - (index % basicDistinctColors.length)]; // Reverse color assignment
        typeColors[type] = color;
    });
    console.log("Array Departments:", arrDepartments);
    for (let department of arrDepartments) {
        console.log('Inserting tab:' + department);
        // Fill tab header with all departments
        document.getElementById("tabs").innerHTML += `<button onClick="switchTab('${department}')">${department}</button>`;
    }
    createKey(typeColors);
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
                    <tr class="days">
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
                        let score = 0;
                        let nurseNum = 0;
                        let invalid = false;
                        for (let nurse of shift.nurses) {
                            if (!departmentPerShift.includes(nurse.department)) {
                                departmentPerShift.push(nurse.department);
                            }
                            if (nurse.nurseID == -1) {
                                invalid = true;
                            }
                            score += nurse.score;
                            nurseNum += 1;
                        }
                        departmentPerShift.sort((a, b) => a.localeCompare(b));
                        for (let dept of departmentPerShift) {
                            let color = departmentColors[dept];
                            cell.innerHTML += `<p style="background-color: ${color}">${dept}</p>`;
                        }

                        if (nurseNum > 0) {
                            score = score / nurseNum;
                        
                            if (score > 1.5) {
                                cell.innerHTML += `<p style="font-size: 2em;">😊</p>`;
                            } else if (score >= 0.75 && score <= 1.5) {
                                cell.innerHTML += `<p style="font-size: 2em;">😐</p>`;
                            } else {
                                cell.innerHTML += `<p style="font-size: 2em;">☹️</p>`;
                            }
                            if (invalid) {
                                cell.innerHTML += `<p style="font-size: 2em;">❌</p>`;
                            }
                        } else {
                            console.log(`No nurses found for department ${department}.`);
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
                        let score = 0;
                        let nurseNum = 0;
                        let invalid = false;
                        for (let nurse of shift.nurses) {
                            if (nurse.department === department) {
                                let color = typeColors[nurse.nurseType];
                                cell.innerHTML += `<p style="background-color: ${color}">${nurse.name}</p>`;
                                score += nurse.score;
                                nurseNum += 1;
                                if (nurse.nurseID == -1) {
                                    invalid = true;
                                }
                            }
                        }
                        if (nurseNum > 0) {
                            score = score / nurseNum;
                        
                            if (score > 1.5) {
                                cell.innerHTML += `<p style="font-size: 2em;">😊</p>`;
                            } else if (score >= 0.75 && score <= 1.5) {
                                cell.innerHTML += `<p style="font-size: 2em;">😐</p>`;
                            } else {
                                cell.innerHTML += `<p style="font-size: 2em;">☹️</p>`;
                            }
                            if (invalid) {
                                cell.innerHTML += `<p style="font-size: 2em;">❌</p>`;
                            }
                        } else {
                            console.log(`No nurses found for department ${department}.`);
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

function createKey(typeColors) {
    // Check if the key already exists
    if (document.querySelector(".key-container")) return;

    // Select the main table container where the key will be added
    const tableContainer = document.getElementById("table-container");

    // Create the key container
    const keyContainer = document.createElement("div");
    keyContainer.className = "key-container";
    keyContainer.style.marginBottom = "10px";
    keyContainer.style.padding = "10px";
    keyContainer.style.border = "1px solid #ccc";
    keyContainer.style.borderRadius = "5px";
    keyContainer.style.backgroundColor = "#f9f9f9";
    keyContainer.style.textAlign = "left";

    // Add a header for the key
    const keyHeader = document.createElement("h2");
    keyHeader.style.marginBottom = "10px";
    keyHeader.textContent = "Key:";
    keyContainer.appendChild(keyHeader);

    // Create a horizontal wrapper for the key items
    const keyWrapper = document.createElement("div");
    keyWrapper.style.display = "flex";
    keyWrapper.style.flexWrap = "wrap";
    keyWrapper.style.gap = "15px";

    // Add nurse types to the key
    Object.entries(typeColors).forEach(([type, color]) => {
        const keyItem = document.createElement("div");
        keyItem.style.display = "flex";
        keyItem.style.alignItems = "center";

        // Add color box
        const colorBox = document.createElement("span");
        colorBox.style.backgroundColor = color;
        colorBox.style.width = "20px";
        colorBox.style.height = "20px";
        colorBox.style.display = "inline-block";
        colorBox.style.marginRight = "5px";

        // Add label
        const label = document.createElement("span");
        label.textContent = type;

        // Append color box and label to the key item
        keyItem.appendChild(colorBox);
        keyItem.appendChild(label);

        // Append key item to wrapper
        keyWrapper.appendChild(keyItem);
    });

    // Add satisfaction scores to the key
    const satisfactionScores = [
        { emoji: "😊", meaning: "High Satisfaction" },
        { emoji: "😐", meaning: "Neutral Satisfaction" },
        { emoji: "☹️", meaning: "Low Satisfaction" },
        { emoji: "❌", meaning: "Invalid Shift" },
    ];

    satisfactionScores.forEach((score) => {
        const keyItem = document.createElement("div");
        keyItem.style.display = "flex";
        keyItem.style.alignItems = "center";

        // Add emoji
        const emojiSpan = document.createElement("span");
        emojiSpan.textContent = score.emoji;
        emojiSpan.style.fontSize = "1.5em";
        emojiSpan.style.marginRight = "5px";

        // Add label
        const label = document.createElement("span");
        label.textContent = score.meaning;

        // Append emoji and label to the key item
        keyItem.appendChild(emojiSpan);
        keyItem.appendChild(label);

        // Append key item to wrapper
        keyWrapper.appendChild(keyItem);
    });

    // Append the wrapper to the key container
    keyContainer.appendChild(keyWrapper);

    // Insert the key container above the table container
    tableContainer.insertAdjacentElement("beforebegin", keyContainer);
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