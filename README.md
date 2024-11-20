# Environmental Monitoring System  

## **Overview**  
This project is an integrated environmental monitoring system developed in C. 
It interacts with a free API to retrieve real-time environmental data, processes the data, and provides alerts when critical threshold is exceeded. 
The system is automated using shell scripts and utilizes modular programming techniques to ensure maintainability and efficiency.

## **Project Team**  
- MUTAHIR AHMED  
- AMNA AHMED  
- ABEER KHAN  

## **Features**  
- Real-time data retrieval using HTTP requests to interact with an API.  
- Storage of raw JSON data and processed metrics in organized files.  
- Real-time notifications and email alerts for critical environmental conditions.  
- Automation of data retrieval, processing, and reporting using shell scripts.  
- Modularized code using header files for better readability and maintainability.  
- Efficient data handling with pointers and dynamic memory allocation.  

## **System Components**  

### **1. API Interaction**  
- Fetches real-time environmental data such as temperature and humidity in JSON format using HTTP requests.  
- Parses and extracts relevant parameters for further processing.

### **2. Data Storage**  
- Raw API data is stored in JSON files.  
- Processed data (e.g., averages, critical values) is saved in `.txt` files for easy readability.

### **3. Alert System**  
- Generates real-time system notifications when thresholds are crossed.  
- Sends automated email alerts to relevant personnel for critical environmental readings.

### **4. Automation**  
- Shell scripts automate periodic data retrieval, processing, and report generation, ensuring seamless monitoring without manual intervention.

### **5. Code Structure**  
- Modularized using header files to organize core functionalities: API interaction, data processing, and alert handling.  
- Pointers and dynamic memory allocation optimize memory usage for large datasets.

---

##**reqirements**  
1. Linux operating system.  
2. GCC compiler for C programming.  
3. `curl` library installed for HTTP requests.  
4. Internet access for API interaction.

## **Results**  

- **Accurate Real-Time Monitoring:**  
  The system successfully retrieves and monitors environmental parameters such as temperature and humidity in real-time.  

- **User-Friendly Display:**  
  Processed data is displayed in an easily readable format, with raw data stored for reference and analysis.  

- **Reliable Alerts and Notifications:**  
  The alert system promptly generates real-time notifications and email alerts when critical thresholds are exceeded.  

- **Efficient Data Handling and Storage:**  
  The program efficiently handles large datasets using dynamic memory allocation and ensures scalability by organizing raw and processed data effectively.  

