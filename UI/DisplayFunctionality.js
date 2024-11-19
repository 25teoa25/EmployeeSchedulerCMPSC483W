let currentTab = "Overall";
function switchTab (tabSelected) {
    currentTab = tabSelected;
    console.log(tabSelected);
    document.getElementById('tabHeader').innerHTML = `${currentTab} Schedule`;
}