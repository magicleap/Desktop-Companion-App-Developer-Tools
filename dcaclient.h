// %BANNER_BEGIN%
// ---------------------------------------------------------------------
// %COPYRIGHT_BEGIN%
//
// Copyright (c) 2020 Magic Leap, Inc. (COMPANY) All Rights Reserved.
// Magic Leap, Inc. Confidential and Proprietary
//
// NOTICE: All information contained herein is, and remains the property
// of COMPANY. The intellectual and technical concepts contained herein
// are proprietary to COMPANY and may be covered by U.S. and Foreign
// Patents, patents in process, and are protected by trade secret or
// copyright law. Dissemination of this information or reproduction of
// this material is strictly forbidden unless prior written permission is
// obtained from COMPANY. Access to the source code contained herein is
// hereby forbidden to anyone except current COMPANY employees, managers
// or contractors who have executed Confidentiality and Non-disclosure
// agreements explicitly covering such access.
//
// The copyright notice above does not evidence any actual or intended
// publication or disclosure of this source code, which includes
// information that is confidential and/or proprietary, and is a trade
// secret, of COMPANY. ANY REPRODUCTION, MODIFICATION, DISTRIBUTION,
// PUBLIC PERFORMANCE, OR PUBLIC DISPLAY OF OR THROUGH USE OF THIS
// SOURCE CODE WITHOUT THE EXPRESS WRITTEN CONSENT OF COMPANY IS
// STRICTLY PROHIBITED, AND IN VIOLATION OF APPLICABLE LAWS AND
// INTERNATIONAL TREATIES. THE RECEIPT OR POSSESSION OF THIS SOURCE
// CODE AND/OR RELATED INFORMATION DOES NOT CONVEY OR IMPLY ANY RIGHTS
// TO REPRODUCE, DISCLOSE OR DISTRIBUTE ITS CONTENTS, OR TO MANUFACTURE,
// USE, OR SELL ANYTHING THAT IT MAY DESCRIBE, IN WHOLE OR IN PART.
//
// %COPYRIGHT_END%
// ---------------------------------------------------------------------
// %BANNER_END%

#pragma once
#include <string>

class dcaclientImpl;

extern "C"
{

class dcaclient {
  public:
    dcaclient();
    ~dcaclient();

/*!
    \brief Connect an instance of the DCA to a local server. Must be run before all other commands. 
    \retval True on success, false if a connection could not be established.
*/

    bool connectToServer();
/*!
    \brief List all connected devices, along with their IP addresses. Devices will be separated with newline characters.
    \param[out] outResponse pointer to char array. This will be initalized by the function, and the response must be freed by the consumer
    \retval True on success, in which case outResponse has useful data. False on failure, in which case outResponse may have an error message
*/
    bool listDevices(char** outResponse);

/*!
    \brief Send a simple message to an application. outResponse will be set by the application if it responds in time.
    \param[in] targetApp package name of the application you wish to send a message to.  
    \param[in] pingMessage string that the target application will be sent
    \param[in] targetML Which ML device to target. If set to empty-string and only one device is connected, that device will be used. 
    \param[out] outResponse pointer to char array. This will be initalized by the function, and the response must be freed by the consumer
    \retval True on success, in which case outResponse has useful data. False on failure, in which case outResponse may have an error message
*/
    bool sendPing(const char* targetApp, const char* pingMessage, const char* targetML, char** outResponse);

/*!
    \brief Send a file from the desktop to the ML device. On success, a transfer ID string will be returned which can be used to track the progress.
    \param[in] targetApp package name of the application you wish to send a file to, or "Documents" for the documents folder.
    \param[in] localPath local filepath on the desktop that will be sent.
    \param[in] remotePath Path on the ML device that will recieve the file.
    \param[in] targetML Which ML device to target. If set to empty-string and only one device is connected, that device will be used. 
    \param[out] outResponse pointer to char array. This will be initalized by the function, and the response must be freed by the consumer
    \retval True on success, in which case outResponse has useful data. False on failure, in which case outResponse may have an error message
*/
    bool sendFile(const char* targetApp,
                  const char* localPath,
                  const char* remotePath,
                  const char* targetML,
                  char** outResponse);

/*!
    \brief Pull a file from the ML device to the desktop. On success, a transfer ID string will be returned which can be used to track the progress.
    \param[in] targetApp package name of the application you wish to get a file from, or "Documents" for the documents folder.
    \param[in] localPath local filepath on the desktop that will be saved to.
    \param[in] remotePath Path on the ML device of a file to pull.
    \param[in] targetML Which ML device to target. If set to empty-string and only one device is connected, that device will be used. 
    \param[out] outResponse pointer to char array. This will be initalized by the function, and the response must be freed by the consumer
    \retval True on success, in which case outResponse has useful data. False on failure, in which case outResponse may have an error message
*/
    bool getFile(const char* targetApp,
                 const char* localPath,
                 const char* remotePath,
                 const char* targetML,
                 char** outResponse);

/*!
    \brief Use the transfer id from sendFile/getFile to check the current file transfer progress.
    \param[in] transferID Transfer ID to check the progress of.
    \param[out] outResponse pointer to char array. This will be initalized by the function, and the response must be freed by the consumer
    \retval True on success, in which case outResponse has useful data. False on failure, in which case outResponse may have an error message
*/
    bool getFileProgress(const char* transferID, char** outResponse);

/*!
    \brief Check the device battery level. On success, the response will be a protobuf encoded string that can be decoded with BeltPackBatteryState.proto
    \param[in] targetML Which ML device to target. If set to empty-string and only one device is connected, that device will be used.
    \param[out] outResponse pointer to char array. This will be initalized by the function, and the response must be freed by the consumer
    \retval True on success, in which case outResponse has useful data. False on failure, in which case outResponse may have an error message
*/

    bool getDeviceBatteryLevel(const char* targetML, char** outResponse);

/*!
    \brief Check the controller battery level. On success, the response will be a protobuf encoded string that can be decoded with ControllerInfo.proto
    \param[in] targetML Which ML device to target. If set to empty-string and only one device is connected, that device will be used.
    \param[out] outResponse pointer to char array. This will be initalized by the function, and the response must be freed by the consumer
    \retval True on success, in which case outResponse has useful data. False on failure, in which case outResponse may have an error message
*/

    bool getControllerBatteryLevel(const char* targetML, char** outResponse);

/*!
    \brief Create a direct socket connection and return the handle for the desktop side of that conneciton. The connection is already encrytped by the DCA.
    \param[in] targetApp package name of the application you wish to send a message to.  
    \param[in] targetML Which ML device to target. If set to empty-string and only one device is connected, that device will be used.
    \retval -1 on failure, else a handle to a valid file descriptor with the target application.
*/
    int getDirectSocket(const char* targetApp, const char* targetML = "");

/*
* For use when you would rather control the creation of the socket yourself.
* Connect a socket to the AF_UNIX address "/var/tmp/dcaserver.sock" (Will add api to expose this and changes later)
* Pass the string from getDirectSocketParams as the VERY FIRST thing into that socket, and wait for a 5 byte string "good". Then you are connected to the app
* IE:
* const char* id = getDirectSocketParams();
*   subSock.sendBytes(id.data(), id.size());
*   subSock.receiveBytes(buffer,5);
* */
/*!
    \brief Provides what is necessary to create a direct socket across process or language process.
        Potentially useful if for example you link to Python and would like python to manage the connection rather than a C file descriptor
    \param[in] targetApp package name of the application you wish to send a message to.
    \param[in] targetML Which ML device to target. If set to empty-string and only one device is connected, that device will be used.
    \retval True on success, in which case outResponse has useful data. False on failure, in which case outResponse may have an error message
*/
    bool getDirectSocketParams(std::string targetApp, std::string targetML, char** outResponse);

/*!
    \brief Get the information necessary to establish a DCA connection.
    \param[out] outResponse pointer to char array. This will be initalized by the function, and the response must be freed by the consumer
    \retval True on success, in which case outResponse has useful data. False on failure, in which case outResponse may have an error message
*/
    bool getQRCodeInfo(char** outResponse);

/*!
    \brief Disconnect the DCA client from the server gracefully.
    \retval 0 on success, -1 if an error occured. 
*/
    int hangup();

/*!
    \brief List all DCA compatible applications currently running and their permissions.  On success, the response will be a protobuf encoded string that can be decoded with ApplicationState.proto
    \param[in] targetML Which ML device to target. If set to empty-string and only one device is connected, that device will be used. 
    \param[out] outResponse pointer to char array. This will be initalized by the function, and the response must be freed by the consumer
    \retval True on success, in which case outResponse has useful data. False on failure, in which case outResponse may have an error message
*/
    bool getActiveApplications(const char* targetML, char** outResponse);

/*!
    \brief List the contents of a directory. On success, the response will be a protobuf encoded string that can be decoded with DirectoryContents.proto
    \param[in] targetApp package name of the application you wish to send a message to.  
    \param[in] remotePath the path at which you wish to scan a directory (Will be prefixed with the target's legal readable path)
    \param[in] targetML Which ML device to target. If set to empty-string and only one device is connected, that device will be used. 
    \param[out] outResponse pointer to char array. This will be initalized by the function, and the response must be freed by the consumer
    \retval True on success, in which case outResponse has useful data. False on failure, in which case outResponse may have an error message
*/
    bool getDir(const char* targetApp, const char* remotePath, const char* targetML, char** outResponse);

/*!
    \brief Delete a file on an ML device
    \param[in] targetApp package name of the application you wish to send a message to.  
    \param[in] remotePath the path at which you wish to delete a file or directory (Will be prefixed with the target's legal writable path)
    \param[in] targetML Which ML device to target. If set to empty-string and only one device is connected, that device will be used. 
    \param[out] outResponse pointer to char array. This will be initalized by the function, and the response must be freed by the consumer
    \retval True on success, in which case outResponse has useful data. False on failure, in which case outResponse may have an error message
*/
    bool deleteFile(const char* targetApp, const char* remotePath, const char* targetML, char** outResponse);

/*!
    \brief Move a file on an ML device
    \param[in] targetApp package name of the application you wish to send a message to.
    \param[in] remotePathFrom the path at which you wish to move a file or directory from (Will be prefixed with the target's legal writable path)
    \param[in] remotePathFrom the path at which you wish to move a file or directory to (Will be prefixed with the target's legal writable path)
    \param[in] targetML Which ML device to target. If set to empty-string and only one device is connected, that device will be used.
    \param[out] outResponse pointer to char array. This will be initalized by the function, and the response must be freed by the consumer
    \retval True on success, in which case outResponse has useful data. False on failure, in which case outResponse may have an error message
*/
    bool moveFile(const char* targetApp,
                  const char* remotePathFrom,
                  const char* remotePathMoveTo,
                  const char* targetML,
                  char** outResponse);

/*!
    \brief Copy a file on an ML device
    \param[in] targetApp package name of the application you wish to send a message to.
    \param[in] remotePathFrom the path at which you wish to copy a file or directory from (Will be prefixed with the target's legal writable path)
    \param[in] remotePathFrom the path at which you wish to copy a file or directory to (Will be prefixed with the target's legal writable path)
    \param[in] targetML Which ML device to target. If set to empty-string and only one device is connected, that device will be used. 
    \param[out] outResponse pointer to char array. This will be initalized by the function, and the response must be freed by the consumer
    \retval True on success, in which case outResponse has useful data. False on failure, in which case outResponse may have an error message
*/
    bool copyFile(const char* targetApp,
                  const char* remotePathFrom,
                  const char* remotePathTo,
                  const char* targetML,
                  char** outResponse);

/*!
    \brief Create a new directory on an ML device
    \param[in] targetApp package name of the application you wish to send a message to.
    \param[in] remotePath the path at which you wish to create a directory from (Will be prefixed with the target's legal writable path)
    \param[in] targetML Which ML device to target. If set to empty-string and only one device is connected, that device will be used.
    \param[out] outResponse pointer to char array. This will be initalized by the function, and the response must be freed by the consumer
    \retval True on success, in which case outResponse has useful data. False on failure, in which case outResponse may have an error message
*/
    bool makeDirectory(const char* targetApp, const char* remotePath, const char* targetML, char** outResponse);

/*!
    \brief Cause an ML device to disconnect from the DCA.
    \param[in] targetML Which ML device to target. If set to empty-string and only one device is connected, that device will be used.
    \param[out] outResponse pointer to char array. This will be initalized by the function, and the response must be freed by the consumer
    \retval True on success, in which case outResponse has useful data. False on failure, in which case outResponse may have an error message
*/
    bool triggerDisconnect(const char* targetML, char** outResponse);

/*!
    \brief Get information about device storage. On success, the response will be a protobuf encoded string that can be decoded with StorageState.proto
    \param[in] targetML Which ML device to target. If set to empty-string and only one device is connected, that device will be used.
    \param[out] outResponse pointer to char array. This will be initalized by the function, and the response must be freed by the consumer
    \retval True on success, in which case outResponse has useful data. False on failure, in which case outResponse may have an error message
*/
    bool getStorageInfo(const char* targetML, char** outResponse);

/*!
    \brief Get information about the connected ML device. On success, the response will be a protobuf encoded string that can be decoded with ApplicationState.proto
    \param[in] targetML Which ML device to target. If set to empty-string and only one device is connected, that device will be used. 
    \param[out] outResponse pointer to char array. This will be initalized by the function, and the response must be freed by the consumer
    \retval True on success, in which case outResponse has useful data. False on failure, in which case outResponse may have an error message
*/
    bool getDeviceInfo(const char* targetML, char** outResponse);

/*!
    \brief Kill the connected DCA server.
*/
    void killServer();

/*!
    \brief Get the version of the DCA service running on the target magic leap device.
    \param[in] targetML Which ML device to target. If set to empty-string and only one device is connected, that device will be used.
    \param[out] outResponse pointer to char array. This will be initalized by the function, and the response must be freed by the consumer
    \retval True on success, in which case outResponse has useful data. False on failure, in which case outResponse may have an error message
*/
    bool getMLDCAVersion(std::string targetML, char** outResponse);

/*!
    \brief Get the version of the ML server running on the PC.
    \param[out] outResponse pointer to char array. This will be initalized by the function, and the response must be freed by the consumer
    \retval True on success, in which case outResponse has useful data. False on failure, in which case outResponse may have an error message
*/
    bool getServerVersion(char** outResponse);

  private:
    dcaclientImpl* impl_;
};
}
