const fileInput = document.getElementById("fileInput");
const selectBtn = document.getElementById("selectBtn");
const fileName = document.getElementById("fileName");
const status = document.getElementById("status");

let selectedFile = null;

selectBtn.addEventListener("click", () => {
    fileInput.click();
});

fileInput.addEventListener("change", (e) => {
    selectedFile = e.target.files[0];
    showFile();
});

function showFile() {
    if (selectedFile) {
        fileName.innerHTML = "Selected File: " + selectedFile.name;
    }
}

// COMPRESS
document.getElementById("compressBtn")
.addEventListener("click", async () => {

    if (!selectedFile) {
        alert("Select file first");
        return;
    }

    status.innerHTML = "Compressing...";

    const formData = new FormData();
    formData.append("file", selectedFile);

    const res = await fetch("/compress", {
        method: "POST",
        body: formData
    });

    const blob = await res.blob();
    const url = window.URL.createObjectURL(blob);

    const a = document.createElement("a");
    a.href = url;
    a.download = "compressed.huff";
    a.click();

    status.innerHTML = "Compression Done!";
});

// DECOMPRESS
document.getElementById("decompressBtn")
.addEventListener("click", async () => {

    if (!selectedFile) {
        alert("Select file first");
        return;
    }

    status.innerHTML = "Decompressing...";

    const formData = new FormData();
    formData.append("file", selectedFile);

    const res = await fetch("/decompress", {
        method: "POST",
        body: formData
    });

    const blob = await res.blob();
    const url = window.URL.createObjectURL(blob);

    const a = document.createElement("a");
    a.href = url;
    a.download = "decompressed.txt";
    a.click();

    status.innerHTML = "Decompression Done!";
});