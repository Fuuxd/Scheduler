#Linux only
#make sure that you make this script executable as in:
# chmod +x webUI.sh

#Go to src directory
cd ./src

#Get graph svg to display in page
echo "Building main..."
g++ main.cpp -o graphCreator
./graphCreator


#Build executable to create schedule
echo "Building scheduleCreator..."
g++ mainJava.cpp -o scheduleCreator

#Start server
echo "Starting server"
pm2 start server.js

pm2 status

