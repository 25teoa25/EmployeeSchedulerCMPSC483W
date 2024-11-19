let currentTab = "Overall";
function switchTab (tabSelected) {
    currentTab = tabSelected;
    console.log(tabSelected);
    document.getElementById('tabHeader').innerHTML = `${currentTab} Schedule`;
}

// When the user clicks on <div>, open the popup
function myFunction() {
  var popup = document.getElementById("myPopup");
  popup.classList.toggle("show");
}
