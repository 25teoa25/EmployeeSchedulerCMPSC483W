let currentTab = "Overall";
function switchTab (tabSelected) {
    currentTab = tabSelected;
    console.log(tabSelected);
    document.getElementById('tabHeader').innerHTML = `${currentTab} Schedule`;
}

function applyColorsToElements(baseClass, colorBaseClass, totalColors) {
  
    //Need to apply all departments to the baseclass parameter which we can make it as an array that automatically gets 
    //updated when a new department gets created. This will assign a color from the list of 20 to each department.
    //It will work nicely if we change each department class to soemthing like "Department-Pediatric", this is because
    //it can assign each class with department in the base of it name a color from the assorted array.
  const elements = document.querySelectorAll(`.${baseClass}`);


  // Iterate over the elements
  elements.forEach((element, index) => {

      // Determine the color class (cycle through 1 to totalColors)
      const colorClass = `${colorBaseClass}-${(index % totalColors) + 1}`;

      // Add the color class to the element
      element.classList.add(colorClass);
  });
}