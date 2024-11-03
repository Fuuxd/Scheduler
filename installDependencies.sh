#Linux only
#make sure that you make this script executable as in:
# chmod +x installDependencies.sh

echo "Updating package index..."
sudo apt update

#Install C++ dependencies
echo "Installing g++..."
sudo apt install -y g++

echo "Installing libboost-all-dev..."
sudo apt install -y libboost-all-dev

echo "Installing graphviz..."
sudo apt install -y graphviz


# Install curl if not already installed
echo "Installing curl..."
sudo apt install -y curl

# Install Node.js (change the setup script version if needed)
echo "Installing Node.js..."
curl -fsSL https://deb.nodesource.com/setup_18.x | sudo -E bash -
sudo apt install -y nodejs

# Verify installation
echo "Verifying Node.js and npm installation..."
node -v
npm -v

# initialize new Node.js 
echo "Initializing Node.js project in src directory..."
cd ./src
npm init -y

#install npm dependencies used in server.js
echo "Installing express..."
npm install express

echo "Installing pm2..."
npm -g install pm2

echo "Now run webUI.sh and then run node server.js in the ./src directory"