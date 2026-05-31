using System;
using System.Collections.Generic;
using System.Collections.Specialized;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Client
{
    /**
     * Defines the message codes used for communication between the client and the server
     */
    public enum MESSAGE_CODES
    {
        SIGN_UP = 10,
        LOG_IN = 11,
        LOG_OUT = 12,
        GET_ROOMS = 13,
        GET_PLAYERS_IN_ROOM = 14,
        JOIN_ROOM = 15,
        CREATE_ROOM = 16,
        HIGH_SCORE = 17,
        GET_STATISTICS = 18
    };

    /**
     * Defines the status codes returned by the server to indicate the result of a request
     */
    public enum STATUSES
    {
        FAILURE,
        SUCCESS,
        INCORRECT_PASSWORD,
        USER_DOES_NOT_EXIST,
        USER_ALREADY_EXISTS,
        USER_ALREADY_LOGGED_IN,
        USER_ALREADY_IN_ROOM,
    };

    /**
     * Contains a mapping of status codes to their corresponding human readable messages
     */
    public static class StatusMessages
    {
        /**
         * Maps status code integers to descriptive error or success messages
         */
        public static readonly Dictionary<int, string> Map =
            new Dictionary<int, string>
        {
            { (int)STATUSES.SUCCESS, "Success" },
            { (int)STATUSES.INCORRECT_PASSWORD, "Incorrect Password" },
            { (int)STATUSES.USER_DOES_NOT_EXIST, "User Doesn't Exist" },
            { (int)STATUSES.USER_ALREADY_EXISTS, "User Already Exists" },
            { (int)STATUSES.USER_ALREADY_LOGGED_IN, "User Already Logged In" },
            { (int)STATUSES.USER_ALREADY_IN_ROOM, "You Are Already In A Room" }
        };
    }
}