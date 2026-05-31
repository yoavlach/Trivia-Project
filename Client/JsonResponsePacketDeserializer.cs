using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;
using System.Text.Json;
using System.Runtime.InteropServices.JavaScript;
using System.Text.Json.Nodes;
using System.Printing.IndexedProperties;

namespace Client
{
    internal class JsonResponsePacketDeserializer
    {
        /**
         * Parses the buffer to extract the login response and retrieves the status value
         * @param buffer the byte array containing the serialized response
         * @return a login response object containing the status
         */
        public static LoginResponse deserializeLoginResponse(byte[] buffer)
        {
            return new LoginResponse { status = parseResponse(buffer).data["status"].GetValue<uint>() };
        }

        /**
         * Parses the buffer to extract the signup response and retrieves the status value
         * @param buffer the byte array containing the serialized response
         * @return a signup response object containing the status
         */
        public static SignupResponse deserializeSignupResponse(byte[] buffer)
        {
            return new SignupResponse { status = parseResponse(buffer).data["status"].GetValue<uint>() };
        }

        /**
         * Parses the buffer to extract the logout response and retrieves the status value
         * @param buffer the byte array containing the serialized response
         * @return a logout response object containing the status
         */
        public static LogoutResponse deserializeLogoutResponse(byte[] buffer)
        {
            return new LogoutResponse { status = parseResponse(buffer).data["status"].GetValue<uint>() };
        }

        /**
         * Parses the buffer to extract the players in the room by splitting the comma separated string
         * @param buffer the byte array containing the serialized response
         * @return a response object containing an array of player names
         */
        public static GetPlayersInRoomResponse deserializeGetPlayersResponse(byte[] buffer)
        {
            JsonObject parsed = parseResponse(buffer).data;
            return new GetPlayersInRoomResponse { players = parsed["PlayersInRoom"].GetValue<string>().Split(",") };
        }

        /**
         * Parses the buffer to extract the join room response and retrieves the status value
         * @param buffer the byte array containing the serialized response
         * @return a join room response object containing the status
         */
        public static JoinRoomResponse deserializeJoinRoomResponse(byte[] buffer)
        {
            return new JoinRoomResponse { status = parseResponse(buffer).data["status"].GetValue<uint>() };
        }

        /**
         * Parses the buffer to extract the create room response retrieving the status and room id
         * @param buffer the byte array containing the serialized response
         * @return a create room response object containing the status and room id
         */
        public static CreateRoomResponse deserializeCreateRoomResponse(byte[] buffer)
        {
            JsonObject parsed = parseResponse(buffer).data;
            return new CreateRoomResponse
            {
                status = parsed["status"].GetValue<uint>(),
                id = parsed["id"].GetValue<int>()
            };
        }

        /**
         * Parses the buffer to extract the available rooms using the helper method to convert the string data
         * @param buffer the byte array containing the serialized response
         * @return a response object containing an array of room data and the status
         */
        public static GetRoomsResponse deserializeGetRoomsResponse(byte[] buffer)
        {
            JsonObject parsed = parseResponse(buffer).data;
            return new GetRoomsResponse
            {
                rooms = Helper.serverMessageToRoomData(parsed["rooms"].GetValue<string>()),
                status = parsed["status"].GetValue<uint>()
            };
        }

        /**
         * Parses the buffer to extract the personal statistics by splitting the comma separated string
         * @param buffer the byte array containing the serialized response
         * @return a response object containing the status and an array of statistics
         */
        public static GetPersonalStatsResponse deserializeGetPersonalStatsResponse(byte[] buffer)
        {
            JsonObject parsed = parseResponse(buffer).data;
            return new GetPersonalStatsResponse { status = parsed["status"].GetValue<uint>(), statistics = parsed["UserStatistics"].GetValue<string>().Split(",") };
        }

        /**
         * Parses the buffer to extract the error response and retrieves the error message
         * @param buffer the byte array containing the serialized response
         * @return an error response object containing the error message
         */
        public static ErrorResponse deserializeErrorResponse(byte[] buffer)
        {
            return new ErrorResponse { message = parseResponse(buffer).data["message"].GetValue<string>() };
        }

        /**
         * Parses the buffer to extract the high scores by iterating through the json array and splitting the username and score
         * @param buffer the byte array containing the serialized response
         * @return a high score response object containing the status and a jagged array of scores
         */
        public static HighScoreResponse deserializeHighScoreResponse(byte[] buffer)
        {
            JsonObject parsed = parseResponse(buffer).data;
            JsonArray arr = parsed["HighScore"].AsArray();
            string[] scores = new string[arr.Count];
            string[][] result = new string[arr.Count][];
            for (int i = 0; i < arr.Count; i++)
                scores[i] = arr[i].GetValue<string>();
            for (int i = 0; i < arr.Count; i++)
                result[i] = arr[i].GetValue<string>().Split(':');

            return new HighScoreResponse { status = parsed["status"].GetValue<uint>(), highScore = result };
        }

        /**
         * Extracts the message code data length and json data from the raw byte buffer to create a general response object
         * @param buffer the raw byte array received from the network
         * @return a general response object containing the message code and parsed json data
         */
        private static GeneralResponse parseResponse(byte[] buffer)
        {
            if (buffer == null || buffer.Length < 5)
                throw new ArgumentException("Invalid buffer");

            int msgCode = buffer[0];

            int dataLength = BitConverter.ToInt32(buffer, 1);

            byte[] dataBytes = new byte[dataLength];
            Buffer.BlockCopy(buffer, 5, dataBytes, 0, dataLength);

            string jsonData = System.Text.Encoding.UTF8.GetString(dataBytes);

            return new GeneralResponse
            {
                msgCode = msgCode,
                data = JsonNode.Parse(jsonData).AsObject()
            };
        }
    }
}
