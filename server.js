const express = require("express");
const multer = require("multer");
const { exec } = require("child_process");
const path = require("path");
const fs = require("fs");

const app = express();
const upload = multer({ dest: "uploads/" });

// serve frontend
app.use(express.static("public"));

// COMPRESS
app.post("/compress", upload.single("file"), (req, res) => {

    const inputPath = req.file.path;
    const outputPath = inputPath + ".huff";

    exec(`g++ huffman.cpp -o huffman && ./huffman compress ${inputPath} ${outputPath}`, (err) => {

        if (err) {
            console.log(err);
            return res.send("Compression failed");
        }

        res.download(outputPath, "compressed.huff", () => {
            fs.unlinkSync(inputPath);
            fs.unlinkSync(outputPath);
        });
    });
});

// DECOMPRESS
app.post("/decompress", upload.single("file"), (req, res) => {

    const inputPath = req.file.path;
    const outputPath = inputPath + ".txt";

    exec(`./huffman decompress ${inputPath} ${outputPath}`, (err) => {

        if (err) {
            console.log(err);
            return res.send("Decompression failed");
        }

        res.download(outputPath, "decompressed.txt", () => {
            fs.unlinkSync(inputPath);
            fs.unlinkSync(outputPath);
        });
    });
});

app.listen(3000, () => {
    console.log("Server running at http://localhost:3000");
});