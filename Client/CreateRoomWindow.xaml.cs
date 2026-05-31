using System;
using System.Collections.Generic;
using System.Linq;
using System.Printing.IndexedProperties;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Shapes;

namespace Client
{
    /// <summary>
    /// Interaction logic for CreateRoomWindow.xaml
    /// </summary>
    public partial class CreateRoomWindow : Window
    {
        /**
         * Initializes the create room window and its components
         */
        public CreateRoomWindow()
        {
            InitializeComponent();
        }

        /**
         * Handles the action when the create room button is clicked
         *  Sends a request to the server with the room details if inputs are valid
         * @param sender the object that triggered the event
         * @param e the event arguments
         */
        public void createButtonPressed(object sender, RoutedEventArgs e)
        {
            int questionTimeValue = 0, maxPlayersValue = 0;
            if (checkInputFields())
            {
                if (!int.TryParse(questionTimeInput.Text, out questionTimeValue) ||
                    !int.TryParse(maxPlayerInput.Text, out maxPlayersValue))
                {
                    MessageBox.Show("Unable to parse message");
                    return;
                }
                handleResponse(Communicator.sendAndReceive(JsonRequestPacketSerializer.serializeRequest(new CreateRoomRequest
                {
                    roomName = roomNameInput.Text,
                    maxUsers = maxPlayersValue,
                    questionCount = 10, //for now
                    answerTimeout = questionTimeValue
                })));
            }
        }

        /**
         * Validates the input fields for the room creation form
         *  Displays error messages to the user if the input is invalid
         * @return true if all inputs are valid false otherwise
         */
        private bool checkInputFields()
        {
            bool valid = false;
            int questionTimeValue = 0, maxPlayersValue = 0;
            if (!int.TryParse(questionTimeInput.Text, out questionTimeValue) || !int.TryParse(maxPlayerInput.Text, out maxPlayersValue))
                MessageBox.Show("Please enter a valid number");
            else if (questionTimeValue < 0 || maxPlayersValue < 0)
                MessageBox.Show("Please enter a positive number");
            else if (roomNameInput.Text.Length == 0)
                MessageBox.Show("Please enter a name for the room");
            else if (roomNameInput.Text.Contains('.') || roomNameInput.Text.Contains(','))
                MessageBox.Show("Room name cannot contain '.' or ','");
            else
                valid = true;
            return valid;
        }

        /**
         * Processes the response received from the server after sending a create room request
         *  Opens the room lobby window on success or displays an error message on failure
         * @param response the byte array containing the server response
         */
        public void handleResponse(byte[] response)
        {
            CreateRoomResponse deserializedResponse;
            ErrorResponse errorResponse;
            try
            {
                switch (Helper.extractMessageCode(response))
                {
                    case (int)MESSAGE_CODES.CREATE_ROOM:
                        deserializedResponse = JsonResponsePacketDeserializer.deserializeCreateRoomResponse(response);
                        switch (deserializedResponse.status)
                        {
                            case (int)STATUSES.FAILURE:
                                MessageBox.Show(StatusMessages.Map[(int)deserializedResponse.status]);
                                break;
                            case (int)STATUSES.SUCCESS:
                                RoomLobbyWindow roomLobbyWindow = new RoomLobbyWindow(deserializedResponse.id);
                                roomLobbyWindow.Show();
                                Close();
                                break;
                        }
                        break;
                    case (int)STATUSES.FAILURE:
                        errorResponse = JsonResponsePacketDeserializer.deserializeErrorResponse(response);
                        MessageBox.Show(errorResponse.message);
                        break;
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show("Unexpected Error: " + ex.Message);
                return;
            }
        }
    }
}