using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Client
{
    struct LoginRequest
    {
        public string username;
        public string password;
    };

    struct SignupRequest
    {
        public string username;
        public string password;
        public string email;
    };

    struct RequestInfo
    {
        public int id;
        public byte[] buffer;
    };

    struct GetPlayersInRoomRequest
    {
        public int roomId;
    };

    struct JoinRoomRequest
    {
        public int roomId;
    };

    struct CreateRoomRequest
    {
        public string roomName;
        public int maxUsers;
        public int questionCount;
        public int answerTimeout;
    };
}