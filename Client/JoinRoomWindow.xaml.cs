using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Automation.Peers;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Interop;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Shapes;

namespace Client
{
    /// <summary>
    /// Interaction logic for JoinRoomWindow.xaml
    /// </summary>
    public partial class JoinRoomWindow : Window
    {
        RoomData[] m_rooms;

        /**
         * Initializes the join room window and fetches the list of available rooms
         */
        public JoinRoomWindow()
        {
            InitializeComponent();
            refresh();
        }

        /**
         * Sends a request to the server to fetch all available rooms and updates the list box
         * Note: Clears the current list and populates it with the names of the received rooms or shows an error message
         */
        private void refresh()
        {
            byte[] resp;
            GetRoomsResponse deserializedResp;
            resp = Communicator.sendAndReceive(JsonRequestPacketSerializer.serializeRequest(MESSAGE_CODES.GET_ROOMS));
            try
            {
                switch (Helper.extractMessageCode(resp))
                {
                    case (int)MESSAGE_CODES.GET_ROOMS:
                        deserializedResp = JsonResponsePacketDeserializer.deserializeGetRoomsResponse(resp);
                        m_rooms = deserializedResp.rooms;
                        roomsListBox.Items.Clear();
                        for (int i = 0; i < m_rooms.Length; i++)
                            roomsListBox.Items.Add(m_rooms[i].name);
                        break;
                    case (int)STATUSES.FAILURE:
                        MessageBox.Show("Unable to get rooms. Please try again");
                        break;
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show("Unexpected Error: " + ex.Message);
                return;
            }
        }

        /**
         * Handles the action when the refresh button is clicked by updating the room list
         * @param sender the object that triggered the event
         * @param e the event arguments
         */
        private void refreshButtonClicked(object sender, EventArgs e)
        {
            refresh();
        }

        /**
         * Handles the action when a room is selected from the list box
         * Note: Enables the join button only if a valid room is selected
         * @param sender the object that triggered the event
         * @param e the event arguments
         */
        private void roomSelected(object sender, EventArgs e)
        {
            joinButton.IsEnabled = roomsListBox.SelectedItem != null;
        }

        /**
         * Handles the action when the join button is clicked
         * Note: Sends a request to join the selected room and opens the room lobby window upon success
         * @param sender the object that triggered the event
         * @param e the event arguments
         */
        private void joinButtonPressed(object sender, EventArgs e)
        {
            if (roomsListBox.SelectedItem == null)
            {
                MessageBox.Show("Room not selected");
                return;
            }
            byte[] resp;
            JoinRoomResponse deserializedResp;
            ErrorResponse errorResponse;
            RoomData selectedRoomData = m_rooms[roomsListBox.SelectedIndex];
            resp = Communicator.sendAndReceive(JsonRequestPacketSerializer.serializeRequest(new JoinRoomRequest { roomId = selectedRoomData.id }));

            try
            {
                switch (Helper.extractMessageCode(resp))
                {
                    case (int)MESSAGE_CODES.JOIN_ROOM:
                        deserializedResp = JsonResponsePacketDeserializer.deserializeJoinRoomResponse(resp);
                        switch (deserializedResp.status)
                        {
                            case (int)STATUSES.SUCCESS:
                                RoomLobbyWindow roomLobbyWindow = new RoomLobbyWindow(selectedRoomData.id);
                                roomLobbyWindow.Show();
                                Close();
                                break;
                            case (int)STATUSES.FAILURE:
                                MessageBox.Show("Unable to join room. Please try again");
                                break;
                            default:
                                MessageBox.Show(StatusMessages.Map[(int)deserializedResp.status]);
                                break;
                        }
                        break;

                    case (int)STATUSES.FAILURE:
                        errorResponse = JsonResponsePacketDeserializer.deserializeErrorResponse(resp);
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