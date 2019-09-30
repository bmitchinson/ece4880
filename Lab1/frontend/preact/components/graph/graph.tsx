import { h, Component } from 'preact';
import { database } from '../firebase/firebase';
import { TrendChart, TimestampArray } from 'preact-charts';

interface MyProps {}
interface MyState {
    dataArray: TimestampArray;
    dataCoverArray: TimestampArray;
}

export default class Graph extends Component<MyProps, MyState> {
    constructor() {
        super();
        const dataArray = [];
        const dataCoverArray = [];
        for (let i = 0; i < 33; i += 1) {
            const time = new Date();
            time.setSeconds(time.getSeconds() - i);
            dataArray.push({
                timestamp: time,
                temp: Math.random() * 50
            });
        }
        for (let i = 33; i < 66; i += 1) {
            const time = new Date();
            time.setSeconds(time.getSeconds() - i);
            dataArray.push({
                timestamp: time,
                temp: -100
            });
        }
        for (let i = 66; i < 99; i += 1) {
            const time = new Date();
            time.setSeconds(time.getSeconds() - i);
            dataArray.push({
                timestamp: time,
                temp: Math.random() * 50
            });
        }
        dataArray.sort((a, b) => (a.timestamp < b.timestamp ? -1 : 1));
        dataCoverArray.push({
            timestamp: dataArray[0].timestamp,
            temp: 0
        });
        dataCoverArray.push({
            timestamp: dataArray[98].timestamp,
            temp: 0
        });
        // TODO: dataCoverLogic
        dataArray.forEach((ele, index, dataArray) => {
            if (
                index !== 0 &&
                ele.temp === -100 &&
                dataArray[index - 1].temp !== -100
            ) {
                console.log('drop @', dataArray[index - 1].timestamp);
                dataCoverArray.push({
                    timestamp: dataArray[index - 1].timestamp.setMilliseconds(
                        dataArray[index - 1].timestamp.getMilliseconds() - 50
                    ),
                    temp: 0
                });
                dataCoverArray.push({
                    timestamp: dataArray[index - 1].timestamp,
                    temp: dataArray[index - 1].temp
                });
                dataCoverArray.push({
                    timestamp: ele.timestamp,
                    temp: 10
                });
            } else if (
                index !== dataArray.length - 1 &&
                ele.temp === -100 &&
                dataArray[index + 1].temp !== -100
            ) {
                console.log('rise @', dataArray[index + 1].timestamp);
                dataCoverArray.push({
                    timestamp: ele.timestamp,
                    temp: 10
                });
                dataCoverArray.push({
                    timestamp: dataArray[index + 1].timestamp,
                    temp: dataArray[index + 1].temp
                });
                dataCoverArray.push({
                    timestamp: dataArray[index + 1].timestamp.setMilliseconds(
                        dataArray[index + 1].timestamp.getMilliseconds() + 50
                    ),
                    temp: 0
                });
            }
        });
        dataCoverArray.sort((a, b) => (a.timestamp < b.timestamp ? -1 : 1));
        console.log('dataArray', dataArray, 'dataCoverArray', dataCoverArray);
        dataArray.map(val => {
            if (val.temp > 50) {
                val.temp = 50;
            }
            if (val.temp < 0) {
                val.temp = 10;
            }
            return val;
        });
        dataCoverArray.map(val => {
            if (val.temp > 50) {
                val.temp = 50;
            }
            if (val.temp < 0) {
                val.temp = 10;
            }
            return val;
        });

        this.state = { dataArray, dataCoverArray };
    }

    componentDidMount() {
        // setInterval(this.addNewPoint, 1000);
    }

    addNewPoint = () => {
        const dataArrayEdit = this.state.dataArray;
        const time = new Date();
        dataArrayEdit.push({
            timestamp: time,
            temp: Math.random() * 60 - 10
        });
        dataArrayEdit.sort((a, b) => (a.timestamp < b.timestamp ? -1 : 1));
        dataArrayEdit.splice(0, 1);
        this.setState({ dataArray: dataArrayEdit });
    };
    // Fetch 300 seconds, reg collection onChange

    // collection on change: grab 1 and add it to stack

    render() {
        const { dataArray, dataCoverArray } = this.state;
        console.log('given:', dataArray);
        dataArray[0].temp = 0;
        dataArray[1].temp = 50;
        return (
            <TrendChart
                lineColour="#90d7c2"
                lineColourTwo="#ffffff"
                name="Temps"
                x="timestamp"
                y="temp"
                data={dataArray}
                dataSetTwo={dataCoverArray}
                margin={{ top: 60, right: 80, left: 80, bottom: 60 }}
                axisControl={false}
                tooltip={false}
            />
        );
    }
}
