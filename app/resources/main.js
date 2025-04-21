let tabCount = 0;

function handleKey(e) {
  if (e.key === "Enter") {
    handleGo();
  }
}

function focusTab(tabId) {
  currentTabId = tabId;
  window.cefAPI.send("focus-tab:" + tabId);
}

let currentTabId = null;
const browsers = {}; // tabId -> browserId or DOM element

function handleGo() {
  const url = document.getElementById("urlInput").value.trim();
  if (!url) return;

  const normalizedUrl = url.startsWith("http") ? url : "https://" + url;

  if (currentTabId !== null) {
    // Send message to navigate the current tab
    window.cefAPI.send("navigate-tab:" + currentTabId + "::" + normalizedUrl);
  } else {
    // No tab selected — fallback to create
    createNewTab(normalizedUrl);
  }
}

function createNewTab(url) {
  const tabId = ++tabCount;
  createTabElement("Tab " + tabId, tabId);
  window.cefAPI.send("create-tab:" + tabId + "::" + url);
  currentTabId = tabId;
}

function createTabElement(name, tabId) {
  const tab = document.createElement("div");
  tab.className = "tab";
  tab.textContent = name;
  tab.onclick = () => {
    currentTabId = tabId;
    window.cefAPI.send("focus-tab:" + tabId);
  };
  document.getElementById("tabBar").appendChild(tab);
}

function updateTabTitle(id, title) {
  const tabs = document.querySelectorAll(".tab");
  const tab = tabs[id];
  if (tab) {
    tab.textContent = title;
  }
}

function sendToCEF(message) {
  if (typeof cefQuery !== "undefined") {
    cefQuery({
      request: message,
      onSuccess: () => {},
      onFailure: (code, msg) => console.error("CEF Error:", msg),
    });
  }
}

let bookmarks = [];

function addBookmark() {
  const currentURL = document.getElementById("urlInput").value;
  bookmarks.push(currentURL);
  renderBookmarks();
}

function renderBookmarks() {
  const container = document.getElementById("bookmarks");
  container.innerHTML = "";

  bookmarks.forEach((url, index) => {
    const link = document.createElement("a");
    link.href = "#";
    link.textContent = url;
    link.onclick = () => {
      document.getElementById("urlInput").value = url;
      handleGo();
    };
    container.appendChild(link);
    container.appendChild(document.createElement("br"));
  });
}
