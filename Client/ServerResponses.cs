using System;
using System.Collections.Generic;
using System.Linq;
using System.Security.Policy;
using System.Text;
using System.Text.Json.Nodes;
using System.Threading.Tasks;

namespace Client
{
    struct LoginResponse
    {
        public uint status;
    };

    struct SignupResponse
    {
        public uint status;
    };

    struct LogoutResponse
    {
        public uint status;
    };

    struct JoinRoomResponse
    {
        public uint status;
    };

    struct CreateRoomResponse
    {
        public uint status;
        public int id;
    };

    struct ErrorResponse
    {
        public string message;
    };

    struct GetRoomsResponse
    {
        public uint status;
        public RoomData[] rooms;
    };

    struct GetPlayersInRoomResponse
    {
        public string[] players;
    };

    struct HighScoreResponse
    {
        public uint status;
        public string[][] highScore;
    };

    struct GetPersonalStatsResponse
    {
        public uint status;
        public string[] statistics;
    };

    struct GeneralResponse
    {
        public int msgCode;
        public JsonObject data;
    };
}