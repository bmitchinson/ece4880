import { h, Component } from 'preact';
import { auth } from '../firebase/firebase';

import Guard from './guard/gaurd';
import Form from './form/form';

// import style from './controls.scss';

interface MyProps {}
interface MyState {
    currentUser: any;
}

export default class Controls extends Component<MyProps, MyState> {
    constructor() {
        super();
        this.state = {
            currentUser: null
        };
    }

    componentDidMount() {
        auth.onAuthStateChanged(currentUser => {
            this.setState({ currentUser });
            // Could have a callback to this that actually verifies their auth status + effects UI
        });
    }

    render() {
        const user = this.state.currentUser;
        return user ? <Form /> : <Guard />;
    }
}
