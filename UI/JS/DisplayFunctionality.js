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
                    const shiftsByType = { 1: [], 2: [], 3: [] };

                    for (let shiftType in shiftsByType) {
                        shiftsByType[shiftType] = Array(14).fill([]);
                    }

                    for (let shift of data) {
                        const shiftType = (shift["1shift"] - 1) % 3 + 1;
                        const dayIndex = Math.floor((shift["1shift"] - 1) / 3);
                        if (dayIndex < 14) {
                            shiftsByType[shiftType][dayIndex] = shiftsByType[shiftType][dayIndex].concat(shift.nurses);
                        }
                    }

                    Object.entries(shiftsByType).forEach(([shiftType, shiftsByDay]) => {
                        let row = document.createElement("tr");
                        if (shiftType == 1) {
                            row.innerHTML = `<th class="shift-row">Shift #1 <br> 7am-3pm</th>`;
                        } else if (shiftType == 2) {
                            row.innerHTML = `<th class="shift-row">Shift #2 <br> 3pm-11pm</th>`;
                        } else if (shiftType == 3) {
                            row.innerHTML = `<th class="shift-row">Shift #3 <br> 11pm-7am</th>`;
                        }

                        shiftsByDay.forEach((nurses, dayIndex) => {
                            let cell = document.createElement("td");
                            let departmentOverall = [];
                            let score = 0;
                            let nurseCount = 0;
                            let invalid = false;

                            nurses.forEach((nurse) => {
                                if (!departmentOverall.includes(nurse.department)) {
                                    departmentOverall.push(nurse.department);
                                }
                                score += nurse.score;
                                nurseCount += 1;
                                if (nurse.nurseID === -1) {
                                    invalid = true;
                                }
                            });

                            departmentOverall.sort((a, b) => a.localeCompare(b));

                            departmentOverall.forEach((dept) => {
                                let color = departmentColors[dept];
                                cell.innerHTML += `<p style="background-color: ${color}">${dept}</p>`;
                            });

                            if (departmentOverall.length === 0) {
                                cell.innerHTML = `<p style="font-style: italic;">No nurses scheduled</p>`;
                            }

                            if (nurseCount > 0) {
                                const avgScore = score / nurseCount;
                
                                if (avgScore > 1.5) {
                                    cell.innerHTML += `<p style="font-size: 2em;">😊</p>`;
                                } else if (avgScore >= 0.75 && avgScore <= 1.5) {
                                    cell.innerHTML += `<p style="font-size: 2em;">😐</p>`;
                                } else {
                                    cell.innerHTML += `<p style="font-size: 2em;">☹️</p>`;
                                }
                                if (invalid) {
                                    cell.innerHTML += `<p style="font-size: 2em;">❌</p>`;
                                }
                            } else {
                                cell.innerHTML = `<p style="font-style: italic;">No nurses scheduled</p>`;
                            }

                            row.appendChild(cell);
                        });

                        tableBody.appendChild(row);
                    });
                }
                else {
                    const shiftsByType = { 1: [], 2: [], 3: [] };

                    for (let shiftType in shiftsByType) {
                        shiftsByType[shiftType] = Array(14).fill([]);
                    }

                    for (let shift of data) {
                        const shiftType = (shift["1shift"] - 1) % 3 + 1;
                        const dayIndex = Math.floor((shift["1shift"] - 1) / 3);
                        if (dayIndex < 14) {
                            shiftsByType[shiftType][dayIndex] = shiftsByType[shiftType][dayIndex].concat(shift.nurses);
                        }
                    }

                    Object.entries(shiftsByType).forEach(([shiftType, shiftsByDay]) => {
                        let row = document.createElement('tr');
                        if (shiftType == 1) {
                            row.innerHTML = `<th class="shift-row">Shift #1 <br> 7am-3pm</th>`;
                        } else if (shiftType == 2) {
                            row.innerHTML = `<th class="shift-row">Shift #2 <br> 3pm-11pm</th>`;
                        } else if (shiftType == 3) {
                            row.innerHTML = `<th class="shift-row">Shift #3 <br> 11pm-7am</th>`;
                        }

                        shiftsByDay.forEach((nurses, dayIndex) => {
                            let cell = document.createElement('td');
                            let score = 0;
                            let nurseNum = 0;
                            let invalid = false;

                            nurses.forEach(nurse => {
                                if (nurse.department === department) {
                                    let color = typeColors[nurse.nurseType];
                                    cell.innerHTML += `<p style="background-color: ${color}">${nurse.name}</p>`;
                                    score += nurse.score;
                                    nurseNum += 1;
                                    if (nurse.nurseID === -1) {
                                        invalid = true;
                                    }
                                }
                            });

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
                                cell.innerHTML = `<p style="font-style: italic;">No nurses</p>`;
                            }

                            row.appendChild(cell);
                        });

                        tableBody.appendChild(row);
                    }
                )
            }
            })
            .catch(error => {
                console.error('Error loading JSON:', error);
            });
    }, 100);
}

function createKey(typeColors) {
    if (document.querySelector(".key-container")) return;

    const tableContainer = document.getElementById("table-container");

    const keyContainer = document.createElement("div");
    keyContainer.className = "key-container";
    keyContainer.style.marginBottom = "10px";
    keyContainer.style.padding = "10px";
    keyContainer.style.border = "1px solid #ccc";
    keyContainer.style.borderRadius = "5px";
    keyContainer.style.backgroundColor = "#f9f9f9";
    keyContainer.style.textAlign = "left";

    const keyHeader = document.createElement("h2");
    keyHeader.style.marginBottom = "10px";
    keyHeader.textContent = "Key:";
    keyContainer.appendChild(keyHeader);

    const keyWrapper = document.createElement("div");
    keyWrapper.style.display = "flex";
    keyWrapper.style.flexWrap = "wrap";
    keyWrapper.style.gap = "15px";

    Object.entries(typeColors).forEach(([type, color]) => {
        const keyItem = document.createElement("div");
        keyItem.style.display = "flex";
        keyItem.style.alignItems = "center";

        const colorBox = document.createElement("span");
        colorBox.style.backgroundColor = color;
        colorBox.style.width = "20px";
        colorBox.style.height = "20px";
        colorBox.style.display = "inline-block";
        colorBox.style.marginRight = "5px";

        const label = document.createElement("span");
        label.textContent = type;

        keyItem.appendChild(colorBox);
        keyItem.appendChild(label);

        keyWrapper.appendChild(keyItem);
    });

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

        const emojiSpan = document.createElement("span");
        emojiSpan.textContent = score.emoji;
        emojiSpan.style.fontSize = "1.5em";
        emojiSpan.style.marginRight = "5px";

        const label = document.createElement("span");
        label.textContent = score.meaning;

        keyItem.appendChild(emojiSpan);
        keyItem.appendChild(label);

        keyWrapper.appendChild(keyItem);
    });

    keyContainer.appendChild(keyWrapper);

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