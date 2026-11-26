from launch import LaunchDescription
from launch_ros.actions import Node


def generate_launch_description():
    publisher_cmd = Node(
        package = 'topic_pkg',
        executable = 'publisher_node',
        name = 'publisher_node'
    )

    subscriber_cmd = Node(
        package = 'topic_pkg',
        executable = 'subscriber_node',
        name = 'subscriber_node'
    )

    Id = LaunchDescription()
    Id.add_action(publisher_cmd)
    Id.add_action(subscriber_cmd)

    return Id
