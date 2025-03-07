import { useState, useEffect } from "react";
import React from 'react';
import ReactDOM from 'react-dom/client';
import io from 'socket.io-client';
import './index.css';

const socket = io('http://localhost:3000');

function Dashboard() {
  const [dateTime, setDateTime] = useState("");
  const [arduinoData, setArduinoData] = useState({
    temperature: 0,
    humidity: 0,
    pulse: 0,
    angle: 0,
    door: 0,
  });
  const [alertMessages, setAlertMessages] = useState([]);

  useEffect(() => {
    const updateDate = () => {
      const options = {
        weekday: "short",
        year: "numeric",
        month: "short",
        day: "numeric",
        hour: "2-digit",
        minute: "2-digit",
        second: "2-digit",
      };
      setDateTime(new Date().toLocaleString("en-US", options));
    };

    updateDate();
    const interval = setInterval(updateDate, 1000);
    return () => clearInterval(interval);
  }, []);

  useEffect(() => {
    socket.on('sensorData', (data) => {
      console.log("Received data:", data); // Debugging log
      setArduinoData({
        temperature: data.temp,
        humidity: data.humidity,
        pulse: data.pulse,
        angle: data.angle,
        door: data.door,
      });

      // Set alert messages based on certain conditions
      const messages = [];
      if (data.temp < 20) {
        messages.push("Temperature is low!");
      }
      if (data.temp > 30) {
        messages.push("Temperature is high!");
      }
      if (data.pulse < 90) {
        messages.push("Pulse is too low!");
      }
      if (data.pulse > 130) {
        messages.push("Pulse is too high!");
      }
      // if (data.angle < 1000) {
      //   messages.push("Bed angle needs adjustment!");
      // }
      if (data.angle < 1000) {
        messages.push("Add the maitress!");
      }
      if (data.door < 900) {
        messages.push("Door is open!");
      }
      setAlertMessages(messages);
    });

    return () => {
      socket.off('sensorData');
    };
  }, []);

  return (
    <div className="bg-background p-6">
      {/* <h1 className="title">Dashboard</h1> */}
      <img src="src/med1.jpg" alt="Medhat Photo" className="circular-image" />
      <div className="flex justify-between">
        <h2>Name: Medhat Metwally</h2>
        <h2>Mother Name: Effat Mamdouh</h2>
      </div>

      <div className="bg-bigcard p-4 rounded-lg">
        <div className="flex justify-between items-center">
          <h1 className="text-2xl font-bold text-pri" id="Showdate">{dateTime}</h1>
          <div className="flex space-x-2">
            <button className="bg-secondary text-button px-4 py-2 rounded-lg">Silence</button>
            <button className="bg-destructive text-button text-white px-4 py-2 rounded-lg">Alarms</button>
          </div>
        </div>

        <div className="mt-4 grid grid-cols-2 gap-4">
          <div className="bg-card p-4 rounded-lg text-center">
            <h2 className="text-primary">Heart Rate</h2>
            <p className="text-4xl font-bold text-accent">{arduinoData.pulse} bpm</p>
            <p className="text-muted">bpm</p>
          </div>
          <div className="bg-card p-4 rounded-lg text-center">
            <h2 className="text-primary">Bed Angle</h2>
            <p className="text-4xl font-bold text-accent">
              {arduinoData.angle > 1000 ? "Normal" : "Needs Adjustment"}
            </p>
          </div>
        </div>

        <div className="mt-4 grid grid-cols-2 gap-4">
          <div className="bg-card p-4 rounded-lg text-center">
            <h2 className="text-primary">Humidity</h2>
            <p className="text-4xl font-bold text-accent">{arduinoData.humidity}%</p>
            <p className="text-muted">%</p>
          </div>
          <div className="bg-card p-4 rounded-lg text-center">
            <h2 className="text-primary">Temperature</h2>
            <p className="text-4xl font-bold text-accent">{arduinoData.temperature}&deg;C</p>
            <p className="text-muted">&deg;C</p>
          </div>
        </div>

        <div className="mt-4 flex justify-between">
          <button className="bg-secondary text-button px-4 py-2 rounded-lg">Menu</button>
          <button className="bg-secondary text-button px-4 py-2 rounded-lg">Home</button>
        </div>
      </div>

      {/* Conditionally render the alert boxes */}
      {alertMessages.map((message, index) => (
        <div key={index} className="alert-box">
          {message}
        </div>
      ))}
    </div>
  );
}

const root = ReactDOM.createRoot(document.getElementById('root'));
root.render(
  <React.StrictMode>
    <Dashboard />
  </React.StrictMode>
);