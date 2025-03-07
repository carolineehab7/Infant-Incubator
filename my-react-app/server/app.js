const path = require("path");
const http = require("http");
const fs = require("fs");
const { SerialPort } = require("serialport"); // Correct import
const { ReadlineParser } = require("@serialport/parser-readline"); // Correct import
const socketIO = require("socket.io");

// Read HTML file
const index = fs.readFileSync(path.join(__dirname, "indexpa.html"));

// Create HTTP server
const app = http.createServer((req, res) => {
  res.writeHead(200, { "Content-Type": "text/html" });
  res.end(index);
});

// Initialize Socket.IO with CORS support
const io = socketIO(app, {
  cors: {
    origin: "*",
    methods: ["GET", "POST"],
  },
});

// Serial port setup
const port = new SerialPort({
  path: "COM3", // Replace with your serial port
  baudRate: 9600,
  dataBits: 8,
  parity: "none",
  stopBits: 1,
  flowControl: false,
});

// Create a parser
const parser = port.pipe(new ReadlineParser({ delimiter: "\r\n" }));

// Handle serial port errors
port.on("error", (err) => {
  console.error("Serial port error:", err);
});

// Function to log data to CSV file
function logDataToCSV(data) {
  const csvLine = `${data.temp},${data.humidity},${data.pulse} \n`;
  fs.appendFile("sensor_data.csv", csvLine, (err) => {
    if (err) {
      console.error("Error writing to CSV file:", err);
    }
  });
}

// Handle serial data
parser.on("data", (data) => {
  try {
    const values = data.split(",").map(Number);
    console.log(values);
    // Validate data length

    const arduinoData = {
      temp: values[0],
      humidity: values[1],
      pulse: values[2],
      angle: values[3],
      door: values[4],
    };

    console.log("Sensor Data:", arduinoData); // Log for debugging
    io.emit("sensorData", arduinoData);

    // Log data to CSV file
    logDataToCSV(arduinoData);
  } catch (error) {
    console.error("Error parsing data:", error);
  }
});

// Handle Socket.IO connections
io.on("connection", (socket) => {
  console.log("Client connected:", socket.id);

  socket.on("disconnect", () => {
    console.log("Client disconnected:", socket.id);
  });
});

// Start server
const PORT = 3000;
app.listen(PORT, () => {
  console.log(`Server running on port ${PORT}`);
});
