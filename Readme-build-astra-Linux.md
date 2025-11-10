# Building an Application for Astra Mobile on aarch64 Architecture

## Requirements

To build an application for **Astra Mobile** on **aarch64** architecture, you need:

– To work on **Astra Linux version 1.7 or 1.8**  
– **Ruby** and **Rhodes** installed  
– A prepared **chroot environment** (see instructions below), and in the file `rhodes/rhobuild.yml` you must specify the path to this chroot  
  Example:
  ```yaml
  env:
    build:
      astraMobileChroot: /home/username/chroot-aarch64
```
– The `build.yml` file of the project must contain the path to the specific Rhodes SDK  
– If you need to enable the **Qt Virtual Keyboard**, add the following to `build.yml`:
```yaml
astraMobile:
    useQtKeyboard: 1
```
---

## Building the Application

Once the environment is ready and configured, you can build the application using the following command:
```bash
rake device:astraMobile:production
```
---
## Preparing the Build Environment

*(Based on the official Astra Linux documentation —  
https://wiki.astralinux.ru/pages/viewpage.action?pageId=323674041)*
### 1. System Requirements

Use **Astra Linux OS 1.7** or **1.8**.

---
### 2. Install Required Tools

The **debootstrap** tool is used to prepare the build environment.  
Install it by running:
```bash
sudo apt install debootstrap qemu-user-static
```
---

### 3. Create the chroot Environment

Run the following command to create a chroot environment:
```bash
sudo debootstrap --arch=arm64 --components=main,contrib,non-free 4.7_arm chroot-arm64 http://download.astralinux.ru/astra/stable/4.7_arm/repository-base/
```
---

### 4. Configure Package Repositories

Edit the file `chroot-arm64/etc/apt/sources.list` and add:
```yaml
deb https://download.astralinux.ru/astra/stable/4.7_arm/repository-base/ 4.7_arm main contrib non-free
deb https://download.astralinux.ru/astra/stable/4.7_arm/repository-extended/ 4.7_arm main contrib non-free
```
---

### 5. Prepare the Build Directory

```bash
echo "export LC_ALL=C" | sudo tee -a chroot-arm64/root/.bashrc
sudo chroot chroot-arm64 apt update
sudo chroot chroot-arm64 apt install devscripts quilt equivs
sudo chown -R "${USER}:${USER}" chroot-arm64/opt/
```
---

### 6. Enter the chroot Environment
```bash
sudo chroot chroot-arm64
```
Then install the required packages:
```bash
apt-get install git gcc g++ make libxslt-dev libxml2-dev zlib1g-dev libz-dev libqt5webenginewidgets5 libqt5webengine5 libqt5webenginecore5 libqt5multimedia5 libqt5multimediawidgets5 libqt5gui5 libqt5concurrent5 libqt5network5 qt5-default qt5-qmake qttools5-dev qtwebengine5-dev qtmultimedia5-dev
```
---

### 7. Configure Environment Variables
```bash
grep -qxF 'export QTDIR=/usr/lib/aarch64-linux-gnu/qt5/' /root/.bashrc || echo 'export QTDIR=/usr/lib/aarch64-linux-gnu/qt5/' >> /root/.bashrc

source /root/.bashrc
```
---

### 8. Install Ruby
```bash
wget https://github.com/postmodern/ruby-install/releases/download/v0.10.1/ruby-install-0.10.1.tar.gz

tar -xzvf ruby-install-0.10.1.tar.gz
cd ruby-install-0.10.1
make install
ruby-install ruby 3.1
```
Add Ruby to the PATH:
```bash
echo 'export PATH="/opt/rubies/ruby-3.1.7/bin:$PATH"' >> /root/.bashrc
source /root/.bashrc
ruby -v
```