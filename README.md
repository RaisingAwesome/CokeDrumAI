# CokeDrumAI

## Introduction
Coke Drum AI was a project developed for the role out of the Avnet Azure Sphere Module Starter Kit on element14.com.

## Element14 Project Page:
https://www.element14.com/community/groups/azuresphere/blog/2019/10/17/avnet-azure-sphere-coke-drum-health-monitoring-iot-ai

## Hackster Project Page:
https://www.hackster.io/raisingawesome/avnet-azure-sphere-coke-drum-health-monitoring-iot-ai-d3f47b

## Makers and Students
So, if you are a maker who just wants to explore cloud based data storage and get an I2C, analog read, and Soft PWM project on the Azure Sphere.  Or if you are a student wanting help getting started on an assignment - you've come to the right place.

Copy down the code in the repository and then follow this blog to get your own Azure Sphere starter kit going:

https://www.element14.com/community/groups/azuresphere/blog/2019/04/24/avnets-azure-sphere-starter-kit-out-of-box-demo-part-1-of-3

To get the DarkSky.net working, you need to create your own account.  I've commented it out for now since you need to update your own "secrets"

Note:  per Microsoft, the ADC for analog input is still in Beta.  So, my code traps for its anomolies it presents.  You'll need to make some tweaks per the following page:

https://docs.microsoft.com/en-us/azure-sphere/app-development/use-beta
