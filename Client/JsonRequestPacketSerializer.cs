using System;
using System.Collections.Generic;
using System.Linq;
using System.Security.AccessControl;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Input;
using System.Text.Json;
using System.Printing.IndexedProperties;

namespace Client
{
    internal class JsonRequestPacketSerializer
    {
        /**
         * Serializes a login request object into a byte array
         * Note: It packages the username and password into an anonymous object and converts it to a JSON string before serializing
         * @param loginRequest the login request object containing the username and password
         * @return a byte array representation of the serialized login request
         */
        public static byte[] serializeRequest(LoginRequest loginRequest)
        {
            var data = new { username = loginRequest.username, password = loginRequest.password };
            return serialize((int)MESSAGE_CODES.LOG_IN, JsonSerializer.Serialize(data));
        }

        /**
         * Serializes a signup request object into a byte array
         * Note: It extracts the username password and email into an anonymous object and turns it into a JSON string
         * @param signupRequest the signup request object containing the user details
         * @return a byte array representation of the serialized signup request
         */
        public static byte[] serializeRequest(SignupRequest signupRequest)
        {
            var data = new { username = signupRequest.username, password = signupRequest.password, email = signupRequest.email };
            return serialize((int)MESSAGE_CODES.SIGN_UP, JsonSerializer.Serialize(data));
        }

        /**
         * Serializes a request to get the players inside a specific room
         * Note: It puts the room ID into an anonymous object and serializes it into a JSON string
         * @param getPlayersInRoomRequest the request object containing the room ID
         * @return a byte array representation of the serialized request
         */
        public static byte[] serializeRequest(GetPlayersInRoomRequest getPlayersInRoomRequest)
        {
            var data = new { roomId = getPlayersInRoomRequest.roomId };
            return serialize((int)MESSAGE_CODES.GET_PLAYERS_IN_ROOM, JsonSerializer.Serialize(data));
        }

        /**
         * Serializes a request to join a specific room
         * Note: It takes the room ID from the request object and converts it to a JSON string
         * @param joinRoomRequest the request object containing the room ID to join
         * @return a byte array representation of the serialized join room request
         */
        public static byte[] serializeRequest(JoinRoomRequest joinRoomRequest)
        {
            var data = new { roomId = joinRoomRequest.roomId };
            return serialize((int)MESSAGE_CODES.JOIN_ROOM, JsonSerializer.Serialize(data));
        }

        /**
         * Serializes a request to create a new room with specific settings
         * Note: It builds an anonymous object with the room name max users question count and timeout then turns it into JSON
         * @param createRoomRequest the request object containing the new room settings
         * @return a byte array representation of the serialized create room request
         */
        public static byte[] serializeRequest(CreateRoomRequest createRoomRequest)
        {
            var data = new
            {
                roomName = createRoomRequest.roomName,
                maxUsers = createRoomRequest.maxUsers,
                questionCount = createRoomRequest.questionCount,
                answerTimeout = createRoomRequest.answerTimeout,
            };
            return serialize((int)MESSAGE_CODES.CREATE_ROOM, JsonSerializer.Serialize(data));
        }

        /**
         * Serializes a basic request that only needs a message code and no extra data
         * Note: It calls the main serialize function passing an empty string for the data part
         * @param msgCode the message code representing the type of request
         * @return a byte array containing just the message code and a zero data length
         */
        public static byte[] serializeRequest(MESSAGE_CODES msgCode)
        {
            return serialize((int)msgCode, "");
        }

        /**
         * Combines a message code data length and JSON data into a single byte array
         * Note: It places the message code in the first byte the data length in the next four bytes and the actual data in the remaining bytes
         * @param msgCode the integer code representing the type of message
         * @param data the JSON string containing the data payload
         * @return a complete byte array ready to be sent over the network
         */
        private static byte[] serialize(int msgCode, string data)
        {
            byte[] dataBytes = Encoding.UTF8.GetBytes(data);
            byte[] result = new byte[1 + 4 + dataBytes.Length];

            result[0] = (byte)msgCode;

            byte[] lengthBytes = BitConverter.GetBytes(dataBytes.Length);
            Array.Copy(lengthBytes, 0, result, 1, 4);

            Array.Copy(dataBytes, 0, result, 5, dataBytes.Length);

            return result;
        }
    }
}