using System;
using System.Net.NetworkInformation;
using System.Net.Sockets;
using System.Xml.Linq;

namespace Client
{
    internal class Helper
    {
        private static int MSG_CODE_SIZE = 1;
        private static int DATA_LEN_PART_SIZE = 4;

        /**
         * Sends data over a network stream and flushes the stream
         * @param stream the network stream to write to
         * @param data the byte array containing the data to send
         */
        public static void sendData(NetworkStream stream, byte[] data)
        {
            stream.Write(data, 0, data.Length);
            stream.Flush();
        }

        /**
         * Reads a complete message from the network stream including its header and data
         *  The header contains the message code and data length
         * @param stream the network stream to read from
         * @return a byte array containing the full message
         */
        public static byte[] getMessage(NetworkStream stream)
        {
            byte[] msg = getPartFromSocket(stream, MSG_CODE_SIZE + DATA_LEN_PART_SIZE);
            int dataLen = extractDataLen(msg);

            byte[] data = getPartFromSocket(stream, dataLen);
            byte[] buffer = new byte[dataLen + MSG_CODE_SIZE + DATA_LEN_PART_SIZE];

            Array.Copy(msg, 0, buffer, 0, msg.Length);
            Array.Copy(data, 0, buffer, msg.Length, data.Length);

            return buffer;
        }

        /**
         * Extracts the message code from the first byte of a message
         * @param message the byte array containing the full message
         * @return the message code as an integer
         */
        public static int extractMessageCode(byte[] message)
        {
            return message[0];
        }

        /**
         * Extracts the length of the data portion of a message starting from the second byte
         * @param message the byte array containing the full message
         * @return the data length as an integer
         */
        public static int extractDataLen(byte[] message)
        {
            return BitConverter.ToInt32(message, 1);
        }

        /**
         * Reads a specific number of bytes from the network stream
         *  Throws an exception if the server disconnects before all bytes are read
         * @param stream the network stream to read from
         * @param bytesNum the number of bytes to read
         * @return a byte array containing the read data or null if the requested bytes count is zero
         */
        private static byte[] getPartFromSocket(NetworkStream stream, int bytesNum)
        {
            if (bytesNum == 0) return null;

            byte[] buffer = new byte[bytesNum];
            int totalRead = 0;

            while (totalRead < bytesNum)
            {
                int read = stream.Read(buffer, totalRead, bytesNum - totalRead);
                if (read == 0) throw new Exception("Server disconnected");
                totalRead += read;
            }

            return buffer;
        }

        /**
         * Parses a formatted string from the server into an array of room data objects
         *  The string should contain comma-separated rooms with dot-separated fields
         * @param serverMsg the string containing the room data from the server
         * @return an array of room data objects parsed from the string
         */
        public static RoomData[] serverMessageToRoomData(string serverMsg)
        {
            if (string.IsNullOrWhiteSpace(serverMsg))
                return new RoomData[0];
            string[] rooms = serverMsg.Split(',', StringSplitOptions.RemoveEmptyEntries);
            RoomData[] result = new RoomData[rooms.Length];
            for (int i = 0; i < rooms.Length; i++)
            {
                string[] fields = rooms[i].Split('.');
                if (fields.Length > (int)ROOM_DATA_INDEXES.TIME_PER_QUESTION + 1)
                    throw new Exception("Invalid format in server response");
                result[i] = new RoomData
                {
                    id = int.Parse(fields[(int)ROOM_DATA_INDEXES.ID]),
                    name = fields[(int)ROOM_DATA_INDEXES.NAME],
                    maxPlayers = uint.Parse(fields[(int)ROOM_DATA_INDEXES.MAX_PLAYERS]),
                    numOfQuestionsInGame = uint.Parse(fields[(int)ROOM_DATA_INDEXES.NUM_OF_QUESTIONS_IN_GAME]),
                    timePerQuestion = uint.Parse(fields[(int)ROOM_DATA_INDEXES.TIME_PER_QUESTION]),
                    status = int.Parse(fields[(int)ROOM_DATA_INDEXES.STATUS])
                };
            }

            return result;
        }
    }
}