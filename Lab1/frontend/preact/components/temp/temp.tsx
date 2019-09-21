import { h, Component } from 'preact';
import { database } from '../firebase/firebase';

const tempRef = database.collection('temps');
const tempObj = tempRef.orderBy('time', 'desc').limit(1);

interface MyProps {}
interface MyState {
    displayTemp: any;
}

export default class Temp extends Component<MyProps, MyState> {
    constructor() {
        super();
        this.state = {
            displayTemp: 0
        };
    }

    componentDidMount() {
        tempObj.get().then(snap => {
            snap.forEach(doc => {
                const temp = doc.data().temp.toFixed(1);
                this.setState({ displayTemp: temp });
            });
        });
    }

    render() {
        return <h2>{this.state.displayTemp}</h2>;
    }
}
