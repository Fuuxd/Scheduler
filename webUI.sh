#Linux only
#make sure that you make this script executable as in:
# chmod +x webUI.sh

#Go to src directory
cd ./src

#Get graph svg to display in page
Echo "Building main..."
g++ main.cpp -o graphCreator
./graphCreator


#Build executable to create schedule
Echo "Building scheduleCreator..."
g++ mainJava.cpp -o scheduleCreator

#Start server
Echo "Starting server"
pm2 start server.js

pm2 status

